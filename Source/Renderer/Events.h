#pragma once

#include <cstdint>
#include <Common/Filesystem.h>
#include <Common/Platform.h>

class Event;
class EventFree;

enum class EventType;
enum class ElementState;
enum class MouseButton;
enum class TouchPhase;
enum class VirtualKeyCode;

using EventPtr = std::unique_ptr<Event, EventFree>;
template <typename T>
inline EventPtr MakeEvent(const T &data);

class Event
{
public:
    virtual ~Event() {}
    
    EventType type;

    struct Resized;
    struct Moved;
    struct DroppedFile;
    struct ReceivedCharacter;
    struct Focused;
    struct KeyboardInput;
    struct MouseMoved;
    struct MouseWheel;
    struct MouseInput;
    struct Touch;

    const Resized &resized() const { return (Resized &)*this; }
    const Moved &moved() const { return (Moved &)*this; }
    const DroppedFile &dropped_file() const { return (DroppedFile &)*this; }
    const ReceivedCharacter &received_character() const { return (ReceivedCharacter &)*this; }
    const Focused &focused() const { return (Focused &)*this; }
    const KeyboardInput &keyboard_input() const { return (KeyboardInput &)*this; }
    const MouseMoved &mouse_moved() const { return (MouseMoved &)*this; }
    const MouseWheel &mouse_wheel() const { return (MouseWheel &)*this; }
    const MouseInput &mouse_input() const { return (MouseInput &)*this; }
    const Touch &touch() const { return (Touch &)*this; }

    // This is the only way to make allocation of this definitely cross-dll-safe
    static void *operator new(size_t size)
    {
        void *ptr;
        ptr = malloc(size);
        if (!ptr)
            throw std::bad_alloc{};
        return ptr;
    }

private:
    template <typename T>
    friend inline EventPtr MakeEvent(const T &data);
    static void operator delete(void *ptr)
    {
        free(ptr);
    }
};

class EventFree
{
public:
    EventFree() : EventFree(&free) {}
    EventFree(void(*pFree)(void *))
        : pFree(pFree)
    {
    }

    void operator()(Event *event)
    {
        event->~Event();
        pFree(event);
    }

private:
    void(*pFree)(void *);
};

template <typename T>
inline EventPtr MakeEvent(const T &data)
{
    T *p = new T(data);
    return EventPtr{ p, EventFree(&free) };
}

struct Event::Resized : public Event
{
    uint32_t width, height;
};

struct Event::Moved : public Event
{
    int32_t x, y;
};

struct Event::DroppedFile : public Event
{
    int32_t x, y;
    fs::path path;
};

struct Event::ReceivedCharacter : public Event
{
    uint32_t codepoint;
};

struct Event::Focused : public Event
{
    bool state;
};

struct Event::KeyboardInput : public Event
{
    ElementState state;
    uint8_t scanCode;

    bool hasVK;
    VirtualKeyCode virtualKey;
};

struct Event::MouseMoved : public Event
{
    int32_t x, y;
};

struct Event::MouseWheel : public Event
{
    bool isPixel;
    float dx, dy;
    TouchPhase touchPhase;
};

struct Event::MouseInput : public Event
{
    ElementState state;
    MouseButton button;
};

struct Event::Touch : public Event
{
    TouchPhase phase;
    double x, y;
    uint64_t id;
};

enum class EventType
{
    Resized,
    Moved,
    Closed,
    DroppedFile,
    ReceivedCharacter,
    Focused,
    KeyboardInput,
    MouseMoved,
    MouseWheel,
    MouseInput,
    Touch,
};

enum class ElementState
{
    Pressed,
    Released,
};

enum class MouseButton
{
    Left,
    Right,
    Middle,
    X1,
    X2,
};

enum class TouchPhase
{
    Started,
    Moved,
    Ended,
    Cancelled,
};

enum class VirtualKeyCode
{
    Key1,
    Key2,
    Key3,
    Key4,
    Key5,
    Key6,
    Key7,
    Key8,
    Key9,
    Key0,

    A,
    B,
    C,
    D,
    E,
    F,
    G,
    H,
    I,
    J,
    K,
    L,
    M,
    N,
    O,
    P,
    Q,
    R,
    S,
    T,
    U,
    V,
    W,
    X,
    Y,
    Z,

    Escape,

    F1,
    F2,
    F3,
    F4,
    F5,
    F6,
    F7,
    F8,
    F9,
    F10,
    F11,
    F12,
    F13,
    F14,
    F15,

    Snapshot,
    Scroll,
    Pause,

    Insert,
    Home,
    Delete,
    End,
    PageDown,
    PageUp,

    Left,
    Up,
    Right,
    Down,

    Back,
    Return,
    Space,

    Numlock,
    Numpad0,
    Numpad1,
    Numpad2,
    Numpad3,
    Numpad4,
    Numpad5,
    Numpad6,
    Numpad7,
    Numpad8,
    Numpad9,

    AbntC1,
    AbntC2,
    Add,
    Apostrophe,
    Apps,
    At,
    Ax,
    Backslash,
    Calculator,
    Capital,
    Colon,
    Comma,
    Convert,
    Decimal,
    Divide,
    Equals,
    Grave,
    Kana,
    Kanji,
    LAlt,
    LBracket,
    LControl,
    LMenu,
    LShift,
    LWin,
    Mail,
    MediaSelect,
    MediaStop,
    Minus,
    Multiply,
    Mute,
    MyComputer,
    NavigateForward,
    NavigateBackward,
    NextTrack,
    NoConvert,
    NumpadComma,
    NumpadEnter,
    NumpadEquals,
    OEM102,
    Period,
    PlayPause,
    Power,
    PrevTrack,
    RAlt,
    RBracket,
    RControl,
    RMenu,
    RShift,
    RWin,
    Semicolon,
    Slash,
    Sleep,
    Stop,
    Subtract,
    Sysrq,
    Tab,
    Underline,
    Unlabeled,
    VolumeDown,
    VolumeUp,
    Wake,
    WebBack,
    WebFavorites,
    WebForward,
    WebHome,
    WebRefresh,
    WebSearch,
    WebStop,
    Yen,
};
