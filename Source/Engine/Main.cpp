#include <Renderer/Renderer.h>
#include <Renderer/RenderHelpers.h>

class TestObj : public IRenderBase
{
public:
};

int main()
{
    // Make an object and a weak reference
    RPtr<IRenderBase> ptr = MakeRenderObject<TestObj>();
    RWeak<IRenderBase> weak = ptr.Downgrade();

    // Try to upgrade the pointer
    RPtr<IRenderBase> upgraded;
    assert(weak.Upgrade(upgraded));

    // Remove all of the strong pointers and ensure upgrade fails
    upgraded.Reset();
    ptr.Reset();
    assert(!weak.Upgrade(upgraded));
}

