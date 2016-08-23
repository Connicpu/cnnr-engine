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

class Event
{
public:
    virtual ~Event() {}

    Event() = default;
    Event(EventType type)
        : type(type)
    {
    };

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

struct Event::Resized : public Event
{
    inline Resized() : Event(EventType::Resized) {}

    uint32_t width, height;
};

struct Event::Moved : public Event
{
    inline Moved() : Event(EventType::Moved) {}

    int32_t x, y;
};

struct Event::DroppedFile : public Event
{
    inline DroppedFile() : Event(EventType::DroppedFile) {}

    int32_t x, y;
    fs::path path;
};

struct Event::ReceivedCharacter : public Event
{
    inline ReceivedCharacter() : Event(EventType::ReceivedCharacter) {}

    uint32_t codepoint;
};

struct Event::Focused : public Event
{
    inline Focused() : Event(EventType::Focused) {}

    bool state;
};

struct Event::KeyboardInput : public Event
{
    inline KeyboardInput() : Event(EventType::KeyboardInput) {}

    ElementState state;
    uint8_t scanCode;

    bool hasVK;
    VirtualKeyCode virtualKey;
};

struct Event::MouseMoved : public Event
{
    inline MouseMoved() : Event(EventType::MouseMoved) {}

    int32_t x, y;
};

struct Event::MouseWheel : public Event
{
    inline MouseWheel() : Event(EventType::MouseWheel) {}

    bool isPixel;
    float dx, dy;
    TouchPhase touchPhase;
};

struct Event::MouseInput : public Event
{
    inline MouseInput() : Event(EventType::MouseInput) {}

    ElementState state;
    MouseButton button;
};

struct Event::Touch : public Event
{
    inline Touch() : Event(EventType::Touch) {}

    TouchPhase phase;
    double x, y;
    uint64_t id;
};

union EventStorage
{
    inline EventStorage()
        : EventStorage(Event{ EventType::Closed })
    {

    }

    #pragma region Copy Initializers
    inline EventStorage(const Event &base)
    {
        new (&this->base) Event(base);
    }

    inline EventStorage(const Event::Resized &resized)
    {
        new (&this->base) Event::Resized(resized);
    }

    inline EventStorage(const Event::Moved &moved)
    {
        new (&this->base) Event::Moved(moved);
    }

    inline EventStorage(const Event::DroppedFile &dropped_file)
    {
        new (&this->base) Event::DroppedFile(dropped_file);
    }

    inline EventStorage(const Event::ReceivedCharacter &received_character)
    {
        new (&this->base) Event::ReceivedCharacter(received_character);
    }

    inline EventStorage(const Event::Focused &focused)
    {
        new (&this->base) Event::Focused(focused);
    }

    inline EventStorage(const Event::KeyboardInput &keyboard_input)
    {
        new (&this->base) Event::KeyboardInput(keyboard_input);
    }

    inline EventStorage(const Event::MouseMoved &mouse_moved)
    {
        new (&this->base) Event::MouseMoved(mouse_moved);
    }

    inline EventStorage(const Event::MouseWheel &mouse_wheel)
    {
        new (&this->base) Event::MouseWheel(mouse_wheel);
    }

    inline EventStorage(const Event::MouseInput &mouse_input)
    {
        new (&this->base) Event::MouseInput(mouse_input);
    }

    inline EventStorage(const Event::Touch &touch)
    {
        new (&this->base) Event::Touch(touch);
    }

    inline EventStorage(const EventStorage &copy)
    {
        switch (copy.base.type)
        {
            case EventType::Resized:
                new (this) EventStorage(copy.resized);
                break;
            case EventType::Moved:
                new (this) EventStorage(copy.moved);
                break;
            case EventType::Closed:
                new (this) EventStorage(copy.base);
                break;
            case EventType::DroppedFile:
                new (this) EventStorage(copy.dropped_file);
                break;
            case EventType::ReceivedCharacter:
                new (this) EventStorage(copy.received_character);
                break;
            case EventType::Focused:
                new (this) EventStorage(copy.focused);
                break;
            case EventType::KeyboardInput:
                new (this) EventStorage(copy.keyboard_input);
                break;
            case EventType::MouseMoved:
                new (this) EventStorage(copy.mouse_moved);
                break;
            case EventType::MouseWheel:
                new (this) EventStorage(copy.mouse_wheel);
                break;
            case EventType::MouseInput:
                new (this) EventStorage(copy.mouse_input);
                break;
            case EventType::Touch:
                new (this) EventStorage(copy.touch);
                break;
            default:
                unreachable();
        }
    }

    inline EventStorage &operator=(const EventStorage &rhs)
    {
        this->~EventStorage();
        new (this) EventStorage(rhs);
        return *this;
    }
#pragma endregion

    #pragma region Move Initializers
    inline EventStorage(Event &&base)
    {
        new (&this->base) Event(base);
    }

    inline EventStorage(Event::Resized &&resized)
    {
        new (&this->base) Event::Resized(std::move(resized));
    }

    inline EventStorage(Event::Moved &&moved)
    {
        new (&this->base) Event::Moved(std::move(moved));
    }

    inline EventStorage(Event::DroppedFile &&dropped_file)
    {
        new (&this->base) Event::DroppedFile(std::move(dropped_file));
    }

    inline EventStorage(Event::ReceivedCharacter &&received_character)
    {
        new (&this->base) Event::ReceivedCharacter(std::move(received_character));
    }

    inline EventStorage(Event::Focused &&focused)
    {
        new (&this->base) Event::Focused(std::move(focused));
    }

    inline EventStorage(Event::KeyboardInput &&keyboard_input)
    {
        new (&this->base) Event::KeyboardInput(std::move(keyboard_input));
    }

    inline EventStorage(Event::MouseMoved &&mouse_moved)
    {
        new (&this->base) Event::MouseMoved(std::move(mouse_moved));
    }

    inline EventStorage(Event::MouseWheel &&mouse_wheel)
    {
        new (&this->base) Event::MouseWheel(std::move(mouse_wheel));
    }

    inline EventStorage(Event::MouseInput &&mouse_input)
    {
        new (&this->base) Event::MouseInput(std::move(mouse_input));
    }

    inline EventStorage(Event::Touch &&touch)
    {
        new (&this->base) Event::Touch(std::move(touch));
    }

    inline EventStorage(EventStorage &&move)
    {
        switch (move.base.type)
        {
            case EventType::Resized:
                new (this) EventStorage(std::move(move.resized));
                break;
            case EventType::Moved:
                new (this) EventStorage(std::move(move.moved));
                break;
            case EventType::Closed:
                new (this) EventStorage(std::move(move.base));
                break;
            case EventType::DroppedFile:
                new (this) EventStorage(std::move(move.dropped_file));
                break;
            case EventType::ReceivedCharacter:
                new (this) EventStorage(std::move(move.received_character));
                break;
            case EventType::Focused:
                new (this) EventStorage(std::move(move.focused));
                break;
            case EventType::KeyboardInput:
                new (this) EventStorage(std::move(move.keyboard_input));
                break;
            case EventType::MouseMoved:
                new (this) EventStorage(std::move(move.mouse_moved));
                break;
            case EventType::MouseWheel:
                new (this) EventStorage(std::move(move.mouse_wheel));
                break;
            case EventType::MouseInput:
                new (this) EventStorage(std::move(move.mouse_input));
                break;
            case EventType::Touch:
                new (this) EventStorage(std::move(move.touch));
                break;
            default:
                unreachable();
        }
    }

    inline EventStorage &operator=(EventStorage &&rhs)
    {
        this->~EventStorage();
        new (this) EventStorage(std::move(rhs));
        return *this;
    }
    #pragma endregion

    inline ~EventStorage()
    {
        base.~Event();
    }

    Event base;
    Event::Resized resized;
    Event::Moved moved;
    Event::DroppedFile dropped_file;
    Event::ReceivedCharacter received_character;
    Event::Focused focused;
    Event::KeyboardInput keyboard_input;
    Event::MouseMoved mouse_moved;
    Event::MouseWheel mouse_wheel;
    Event::MouseInput mouse_input;
    Event::Touch touch;
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
    Key0,
    Key1,
    Key2,
    Key3,
    Key4,
    Key5,
    Key6,
    Key7,
    Key8,
    Key9,

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
