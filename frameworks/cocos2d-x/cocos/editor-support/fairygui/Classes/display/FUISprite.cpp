#include "FUISprite.h"

NS_FGUI_BEGIN
USING_NS_CC;

#define kProgressTextureCoordsCount 4
//  kProgressTextureCoords holds points {0,1} {0,0} {1,0} {1,1} we can represent it as bits
const char kProgressTextureCoords = 0x4b;

Texture2D* FUISprite::_empty = nullptr;

FUISprite::FUISprite() :
    _fillMethod(FillMethod::None),
    _fillOrigin(FillOrigin::Left),
    _fillAmount(0),
    _fillClockwise(false),
    _vertexDataCount(0),
    _vertexData(nullptr),
    _fillGlProgramState(nullptr)
{
}

FUISprite::~FUISprite()
{
    CC_SAFE_FREE(_vertexData);
    CC_SAFE_RELEASE(_fillGlProgramState);
}

void FUISprite::clearContent()
{
    setTexture(nullptr);
    CC_SAFE_RELEASE_NULL(_spriteFrame);
    setCenterRectNormalized(Rect(0, 0, 1, 1));

    _empty = _texture;
}

void FUISprite::setScale9Grid(Rect * value)
{
    if (value == nullptr)
    {
        setCenterRectNormalized(Rect(0, 0, 1, 1));
        return;
    }

    Rect insets = *value;

    // When Insets == Zero --> we should use a 1/3 of its untrimmed size
    if (insets.equals(Rect::ZERO)) {
        insets = Rect(_originalContentSize.width / 3.0f,
            _originalContentSize.height / 3.0f,
            _originalContentSize.width / 3.0f,
            _originalContentSize.height / 3.0f);
    }

    // emulate invalid insets. shouldn't be supported, but the original code supported it.
    if (insets.origin.x > _originalContentSize.width)
        insets.origin.x = 0;
    if (insets.origin.y > _originalContentSize.height)
        insets.origin.y = 0;
    if (insets.size.width > _originalContentSize.width)
        insets.size.width = 1;
    if (insets.size.height > _originalContentSize.height)
        insets.size.height = 1;

    // we have to convert from untrimmed to trimmed
    // Sprite::setCenterRect is using trimmed values (to be compatible with Cocos Creator)
    // Scale9Sprite::setCapInsects uses untrimmed values (which makes more sense)

    // use _rect coordinates. recenter origin to calculate the
    // intersecting rectangle
    // can't use _offsetPosition since it is calculated using bottom-left as origin,
    // and the center rect is calculated using top-left
    insets.origin.x -= (_originalContentSize.width - _rect.size.width) / 2 + _unflippedOffsetPositionFromCenter.x;
    insets.origin.y -= (_originalContentSize.height - _rect.size.height) / 2 - _unflippedOffsetPositionFromCenter.y;

    // intersecting rectangle
    const float x1 = std::max(insets.origin.x, 0.0f);
    const float y1 = std::max(insets.origin.y, 0.0f);
    const float x2 = std::min(insets.origin.x + insets.size.width, 0.0f + _rect.size.width);
    const float y2 = std::min(insets.origin.y + insets.size.height, 0.0f + _rect.size.height);

    // centerRect uses the trimmed frame origin as 0,0.
    // so, recenter inset rect
    insets.setRect(x1,
        y1,
        x2 - x1,
        y2 - y1);

    // Only update center rect while in slice mode.
    setCenterRect(insets);
}

void FUISprite::setGrayed(bool value)
{
    GLProgramState *glState = nullptr;
    if (value)
        glState = GLProgramState::getOrCreateWithGLProgramName(GLProgram::SHADER_NAME_POSITION_GRAYSCALE, getTexture());
    else
        glState = GLProgramState::getOrCreateWithGLProgramName(GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR_NO_MVP, getTexture());

    setGLProgramState(glState);
}

void FUISprite::setFillMethod(FillMethod value)
{
    if (_fillMethod != value) {
        _fillMethod = value;

        if (_fillMethod != FillMethod::None)
            setupFill();
        else
        {
            CC_SAFE_FREE(_vertexData);
            CC_SAFE_RELEASE_NULL(_fillGlProgramState);
        }
    }
}

void FUISprite::setFillOrigin(FillOrigin value)
{
    if (_fillOrigin != value) {
        _fillOrigin = value;

        if (_fillMethod != FillMethod::None)
            setupFill();
    }
}

void FUISprite::setFillClockwise(bool value)
{
    if (_fillClockwise != value) {
        _fillClockwise = value;

        if (_fillMethod != FillMethod::None)
            setupFill();
    }
}

void FUISprite::setFillAmount(float value)
{
    if (_fillAmount != value) {
        _fillAmount = value;

        if (_fillMethod != FillMethod::None)
            setupFill();
    }
}

void FUISprite::setupFill()
{
    if (!_fillGlProgramState)
    {
        _fillGlProgramState = GLProgramState::getOrCreateWithGLProgramName(GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR, getTexture());
        CC_SAFE_RETAIN(_fillGlProgramState);
    }
    if (_fillMethod == FillMethod::Horizontal || _fillMethod == FillMethod::Vertical)
        updateBar();
    else
        updateRadial();
}

//from CCFUISprite.h
///
//    @returns the vertex position from the texture coordinate
///
Tex2F FUISprite::textureCoordFromAlphaPoint(Vec2 alpha)
{
    Tex2F ret(0.0f, 0.0f);

    V3F_C4B_T2F_Quad quad = getQuad();
    Vec2 min(quad.bl.texCoords.u, quad.bl.texCoords.v);
    Vec2 max(quad.tr.texCoords.u, quad.tr.texCoords.v);
    //  Fix bug #1303 so that progress timer handles sprite frame texture rotation
    if (isTextureRectRotated()) {
        std::swap(alpha.x, alpha.y);
    }
    return Tex2F(min.x * (1.f - alpha.x) + max.x * alpha.x, min.y * (1.f - alpha.y) + max.y * alpha.y);
}

Vec2 FUISprite::vertexFromAlphaPoint(Vec2 alpha)
{
    Vec2 ret(0.0f, 0.0f);

    V3F_C4B_T2F_Quad quad = getQuad();
    Vec2 min(quad.bl.vertices.x, quad.bl.vertices.y);
    Vec2 max(quad.tr.vertices.x, quad.tr.vertices.y);
    ret.x = min.x * (1.f - alpha.x) + max.x * alpha.x;
    ret.y = min.y * (1.f - alpha.y) + max.y * alpha.y;
    return ret;
}

void FUISprite::updateColor(void)
{
    Sprite::updateColor();

    if (_vertexData)
    {
        Color4B sc = getQuad().tl.colors;
        for (int i = 0; i < _vertexDataCount; ++i)
        {
            _vertexData[i].colors = sc;
        }
    }
}

///
//    Update does the work of mapping the texture onto the triangles
//    It now doesn't occur the cost of free/alloc data every update cycle.
//    It also only changes the percentage point but no other points if they have not
//    been modified.
//    
//    It now deals with flipped texture. If you run into this problem, just use the
//    sprite property and enable the methods flipX, flipY.
///
void FUISprite::updateRadial(void)
{
    float angle = 2.f*((float)M_PI) * (_fillClockwise ? (1.0f - _fillAmount) : _fillAmount);

    //    We find the vector to do a hit detection based on the percentage
    //    We know the first vector is the one @ 12 o'clock (top,mid) so we rotate
    //    from that by the progress angle around the _midpoint pivot
    Vec2 midpoint(0.5f, 0.5f);
    Vec2 topMid(0.5f, 1.f);
    Vec2 percentagePt = topMid.rotateByAngle(midpoint, angle);

    int index = 0;
    Vec2 hit;

    if (_fillAmount == 0.f) {
        //    More efficient since we don't always need to check intersection
        //    If the alpha is zero then the hit point is top mid and the index is 0.
        hit = topMid;
        index = 0;
    }
    else if (_fillAmount == 1.f) {
        //    More efficient since we don't always need to check intersection
        //    If the alpha is one then the hit point is top mid and the index is 4.
        hit = topMid;
        index = 4;
    }
    else {
        //    We run a for loop checking the edges of the texture to find the
        //    intersection point
        //    We loop through five points since the top is split in half

        float min_t = FLT_MAX;

        for (int i = 0; i <= kProgressTextureCoordsCount; ++i) {
            int pIndex = (i + (kProgressTextureCoordsCount - 1)) % kProgressTextureCoordsCount;

            Vec2 edgePtA = boundaryTexCoord(i % kProgressTextureCoordsCount);
            Vec2 edgePtB = boundaryTexCoord(pIndex);

            //    Remember that the top edge is split in half for the 12 o'clock position
            //    Let's deal with that here by finding the correct endpoints
            if (i == 0) {
                edgePtB = edgePtA.lerp(edgePtB, 1 - midpoint.x);
            }
            else if (i == 4) {
                edgePtA = edgePtA.lerp(edgePtB, 1 - midpoint.x);
            }

            //    s and t are returned by ccpLineIntersect
            float s = 0, t = 0;
            if (Vec2::isLineIntersect(edgePtA, edgePtB, midpoint, percentagePt, &s, &t))
            {

                //    Since our hit test is on rays we have to deal with the top edge
                //    being in split in half so we have to test as a segment
                if ((i == 0 || i == 4)) {
                    //    s represents the point between edgePtA--edgePtB
                    if (!(0.f <= s && s <= 1.f)) {
                        continue;
                    }
                }
                //    As long as our t isn't negative we are at least finding a
                //    correct hitpoint from _midpoint to percentagePt.
                if (t >= 0.f) {
                    //    Because the percentage line and all the texture edges are
                    //    rays we should only account for the shortest intersection
                    if (t < min_t) {
                        min_t = t;
                        index = i;
                    }
                }
            }
        }

        //    Now that we have the minimum magnitude we can use that to find our intersection
        hit = midpoint + ((percentagePt - midpoint) * min_t);
    }

    //    The size of the vertex data is the index from the hitpoint
    //    the 3 is for the _midpoint, 12 o'clock point and hitpoint position.

    bool sameIndexCount = true;
    if (_vertexDataCount != index + 3) {
        sameIndexCount = false;
        CC_SAFE_FREE(_vertexData);
        _vertexDataCount = 0;
    }

    if (!_vertexData) {
        _vertexDataCount = index + 3;
        _vertexData = (V2F_C4B_T2F*)malloc(_vertexDataCount * sizeof(V2F_C4B_T2F));
        CCASSERT(_vertexData, "FUISprite. Not enough memory");
    }
    updateColor();

    if (!sameIndexCount) {

        //    First we populate the array with the _midpoint, then all
        //    vertices/texcoords/colors of the 12 'o clock start and edges and the hitpoint
        _vertexData[0].texCoords = textureCoordFromAlphaPoint(midpoint);
        _vertexData[0].vertices = vertexFromAlphaPoint(midpoint);

        _vertexData[1].texCoords = textureCoordFromAlphaPoint(topMid);
        _vertexData[1].vertices = vertexFromAlphaPoint(topMid);

        for (int i = 0; i < index; ++i) {
            Vec2 alphaPoint = boundaryTexCoord(i);
            _vertexData[i + 2].texCoords = textureCoordFromAlphaPoint(alphaPoint);
            _vertexData[i + 2].vertices = vertexFromAlphaPoint(alphaPoint);
        }
    }

    //    hitpoint will go last
    _vertexData[_vertexDataCount - 1].texCoords = textureCoordFromAlphaPoint(hit);
    _vertexData[_vertexDataCount - 1].vertices = vertexFromAlphaPoint(hit);
}

///
//    Update does the work of mapping the texture onto the triangles for the bar
//    It now doesn't occur the cost of free/alloc data every update cycle.
//    It also only changes the percentage point but no other points if they have not
//    been modified.
//    
//    It now deals with flipped texture. If you run into this problem, just use the
//    sprite property and enable the methods flipX, flipY.
///
void FUISprite::updateBar(void)
{
    Vec2 min, max;

    if (_fillMethod == FillMethod::Horizontal)
    {
        if (_fillOrigin == FillOrigin::Left || _fillOrigin == FillOrigin::Top)
        {
            min = Vec2(0, 0);
            max = Vec2(_fillAmount, 1);
        }
        else
        {
            min = Vec2(1 - _fillAmount, 0);
            max = Vec2(1, 1);
        }
    }
    else
    {
        if (_fillOrigin == FillOrigin::Left || _fillOrigin == FillOrigin::Top)
        {
            min = Vec2(0, 1 - _fillAmount);
            max = Vec2(1, 1);
        }
        else
        {
            min = Vec2(0, 0);
            max = Vec2(1, _fillAmount);
        }
    }

    if (!_vertexData) {
        _vertexDataCount = 4;
        _vertexData = (V2F_C4B_T2F*)malloc(_vertexDataCount * sizeof(V2F_C4B_T2F));
        CCASSERT(_vertexData, "FUISprite. Not enough memory");
    }
    //    TOPLEFT
    _vertexData[0].texCoords = textureCoordFromAlphaPoint(Vec2(min.x, max.y));
    _vertexData[0].vertices = vertexFromAlphaPoint(Vec2(min.x, max.y));

    //    BOTLEFT
    _vertexData[1].texCoords = textureCoordFromAlphaPoint(Vec2(min.x, min.y));
    _vertexData[1].vertices = vertexFromAlphaPoint(Vec2(min.x, min.y));

    //    TOPRIGHT
    _vertexData[2].texCoords = textureCoordFromAlphaPoint(Vec2(max.x, max.y));
    _vertexData[2].vertices = vertexFromAlphaPoint(Vec2(max.x, max.y));

    //    BOTRIGHT
    _vertexData[3].texCoords = textureCoordFromAlphaPoint(Vec2(max.x, min.y));
    _vertexData[3].vertices = vertexFromAlphaPoint(Vec2(max.x, min.y));

    updateColor();
}

Vec2 FUISprite::boundaryTexCoord(char index)
{
    if (index < kProgressTextureCoordsCount) {
        if (!_fillClockwise) {
            return Vec2((kProgressTextureCoords >> (7 - (index << 1))) & 1, (kProgressTextureCoords >> (7 - ((index << 1) + 1))) & 1);
        }
        else {
            return Vec2((kProgressTextureCoords >> ((index << 1) + 1)) & 1, (kProgressTextureCoords >> (index << 1)) & 1);
        }
    }
    return Vec2::ZERO;
}

void FUISprite::onDraw(const Mat4 &transform, uint32_t /*flags*/)
{
    GLProgram* program = _fillGlProgramState->getGLProgram();
    program->use();
    program->setUniformsForBuiltins(transform);

    GL::blendFunc(getBlendFunc().src, getBlendFunc().dst);

    GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POS_COLOR_TEX);

    GL::bindTexture2D(getTexture());

    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(_vertexData[0]), &_vertexData[0].vertices);
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORD, 2, GL_FLOAT, GL_FALSE, sizeof(_vertexData[0]), &_vertexData[0].texCoords);
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(_vertexData[0]), &_vertexData[0].colors);

    if (_fillMethod == FillMethod::Horizontal || _fillMethod == FillMethod::Vertical)
    {
        glDrawArrays(GL_TRIANGLE_STRIP, 0, _vertexDataCount);
        CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1, _vertexDataCount);
    }
    else
    {
        glDrawArrays(GL_TRIANGLE_FAN, 0, _vertexDataCount);
        CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1, _vertexDataCount);
    }
}

void FUISprite::draw(cocos2d::Renderer * renderer, const cocos2d::Mat4 & transform, uint32_t flags)
{
    if (_texture == _empty)
        return;

    if (_fillMethod == FillMethod::None)
        Sprite::draw(renderer, transform, flags);
    else
    {
        _customCommand.init(_globalZOrder, transform, flags);
        _customCommand.func = CC_CALLBACK_0(FUISprite::onDraw, this, transform, flags);
        renderer->addCommand(&_customCommand);
    }
}

NS_FGUI_END