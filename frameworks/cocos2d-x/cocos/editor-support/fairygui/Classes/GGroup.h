#ifndef __GGROUP_H__
#define __GGROUP_H__

#include "cocos2d.h"
#include "FairyGUIMacros.h"
#include "GObject.h"

NS_FGUI_BEGIN

class GGroup : public GObject
{
public:
    GGroup();
    virtual ~GGroup();

    CREATE_FUNC(GGroup);

    GroupLayoutType getLayout() { return _layout; }
    void setLayout(GroupLayoutType value);

    int getColumnGap() { return _columnGap; }
    void setColumnGap(int value);

    int getLineGap() { return _lineGap; }
    void setLineGap(int value);

    void setBoundsChangedFlag(bool childSizeChanged = false);
    void moveChildren(float dx, float dy);
    void resizeChildren(float dw, float dh);

    int _updating;

protected:
    virtual void setup_beforeAdd(ByteBuffer* buffer, int beginPos) override;
    virtual void setup_afterAdd(ByteBuffer* buffer, int beginPos) override;
    virtual void handleAlphaChanged() override;
    virtual void handleVisibleChanged() override;

private:
    void updateBounds();
    void handleLayout();
    void updatePercent();
    CALL_LATER_FUNC(GGroup, ensureBoundsCorrect);

    GroupLayoutType _layout;
    int _lineGap;
    int _columnGap;
    bool _percentReady;
    bool _boundsChanged;
};

NS_FGUI_END

#endif
