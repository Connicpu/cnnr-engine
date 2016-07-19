#pragma once

#include "Events.h"
#include <Common/EnumStrings.h>
#include <Common/String.h>

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4307)
#endif

inline const char *EventTypeName(EventType event)
{
    switch (event)
    {
        case EventType::Resized: return "Resized";
        case EventType::Moved: return "Moved";
        case EventType::Closed: return "Closed";
        case EventType::DroppedFile: return "DroppedFile";
        case EventType::ReceivedCharacter: return "ReceivedCharacter";
        case EventType::Focused: return "Focused";
        case EventType::KeyboardInput: return "KeyboardInput";
        case EventType::MouseMoved: return "MouseMoved";
        case EventType::MouseWheel: return "MouseWheel";
        case EventType::MouseInput: return "MouseInput";
        case EventType::Touch: return "Touch";
        default:
            unreachable();
    }
}

inline std::optional<EventType> ParseEventType(const String &name)
{
    BEGIN_STRING_TO_ENUM(name);
    RETURN_STRING_ENUM(name, "Resized", EventType::Resized);
    RETURN_STRING_ENUM(name, "Moved", EventType::Moved);
    RETURN_STRING_ENUM(name, "Closed", EventType::Closed);
    RETURN_STRING_ENUM(name, "DroppedFile", EventType::DroppedFile);
    RETURN_STRING_ENUM(name, "ReceivedCharacter", EventType::ReceivedCharacter);
    RETURN_STRING_ENUM(name, "Focused", EventType::Focused);
    RETURN_STRING_ENUM(name, "KeyboardInput", EventType::KeyboardInput);
    RETURN_STRING_ENUM(name, "MouseMoved", EventType::MouseMoved);
    RETURN_STRING_ENUM(name, "MouseWheel", EventType::MouseWheel);
    RETURN_STRING_ENUM(name, "MouseInput", EventType::MouseInput);
    RETURN_STRING_ENUM(name, "Touch", EventType::Touch);
    END_STRING_TO_ENUM();
}

inline const char *TouchPhaseName(TouchPhase phase)
{
    switch (phase)
    {
        case TouchPhase::Started: return "Started";
        case TouchPhase::Moved: return "Moved";
        case TouchPhase::Ended: return "Ended";
        case TouchPhase::Cancelled: return "Cancelled";
        default:
            unreachable();
    }
}

inline std::optional<TouchPhase> ParseTouchPhase(const String &name)
{
    BEGIN_STRING_TO_ENUM(name);
    RETURN_STRING_ENUM(name, "Started", TouchPhase::Started);
    RETURN_STRING_ENUM(name, "Moved", TouchPhase::Moved);
    RETURN_STRING_ENUM(name, "Ended", TouchPhase::Ended);
    RETURN_STRING_ENUM(name, "Cancelled", TouchPhase::Cancelled);
    END_STRING_TO_ENUM();
}

inline const char *VirtualKeyCodeName(VirtualKeyCode code)
{
    switch (code)
    {
        case VirtualKeyCode::Key0: return "Key0";
        case VirtualKeyCode::Key1: return "Key1";
        case VirtualKeyCode::Key2: return "Key2";
        case VirtualKeyCode::Key3: return "Key3";
        case VirtualKeyCode::Key4: return "Key4";
        case VirtualKeyCode::Key5: return "Key5";
        case VirtualKeyCode::Key6: return "Key6";
        case VirtualKeyCode::Key7: return "Key7";
        case VirtualKeyCode::Key8: return "Key8";
        case VirtualKeyCode::Key9: return "Key9";

        case VirtualKeyCode::A: return "A";
        case VirtualKeyCode::B: return "B";
        case VirtualKeyCode::C: return "C";
        case VirtualKeyCode::D: return "D";
        case VirtualKeyCode::E: return "E";
        case VirtualKeyCode::F: return "F";
        case VirtualKeyCode::G: return "G";
        case VirtualKeyCode::H: return "H";
        case VirtualKeyCode::I: return "I";
        case VirtualKeyCode::J: return "J";
        case VirtualKeyCode::K: return "K";
        case VirtualKeyCode::L: return "L";
        case VirtualKeyCode::M: return "M";
        case VirtualKeyCode::N: return "N";
        case VirtualKeyCode::O: return "O";
        case VirtualKeyCode::P: return "P";
        case VirtualKeyCode::Q: return "Q";
        case VirtualKeyCode::R: return "R";
        case VirtualKeyCode::S: return "S";
        case VirtualKeyCode::T: return "T";
        case VirtualKeyCode::U: return "U";
        case VirtualKeyCode::V: return "V";
        case VirtualKeyCode::W: return "W";
        case VirtualKeyCode::X: return "X";
        case VirtualKeyCode::Y: return "Y";
        case VirtualKeyCode::Z: return "Z";

        case VirtualKeyCode::Escape: return "Escape";

        case VirtualKeyCode::F1: return "F1";
        case VirtualKeyCode::F2: return "F2";
        case VirtualKeyCode::F3: return "F3";
        case VirtualKeyCode::F4: return "F4";
        case VirtualKeyCode::F5: return "F5";
        case VirtualKeyCode::F6: return "F6";
        case VirtualKeyCode::F7: return "F7";
        case VirtualKeyCode::F8: return "F8";
        case VirtualKeyCode::F9: return "F9";
        case VirtualKeyCode::F10: return "F10";
        case VirtualKeyCode::F11: return "F11";
        case VirtualKeyCode::F12: return "F12";
        case VirtualKeyCode::F13: return "F13";
        case VirtualKeyCode::F14: return "F14";
        case VirtualKeyCode::F15: return "F15";

        case VirtualKeyCode::Snapshot: return "Snapshot";
        case VirtualKeyCode::Scroll: return "Scroll";
        case VirtualKeyCode::Pause: return "Pause";

        case VirtualKeyCode::Insert: return "Insert";
        case VirtualKeyCode::Home: return "Home";
        case VirtualKeyCode::Delete: return "Delete";
        case VirtualKeyCode::End: return "End";
        case VirtualKeyCode::PageDown: return "PageDown";
        case VirtualKeyCode::PageUp: return "PageUp";

        case VirtualKeyCode::Left: return "Left";
        case VirtualKeyCode::Up: return "Up";
        case VirtualKeyCode::Right: return "Right";
        case VirtualKeyCode::Down: return "Down";

        case VirtualKeyCode::Back: return "Back";
        case VirtualKeyCode::Return: return "Return";
        case VirtualKeyCode::Space: return "Space";

        case VirtualKeyCode::Numlock: return "Numlock";
        case VirtualKeyCode::Numpad0: return "Numpad0";
        case VirtualKeyCode::Numpad1: return "Numpad1";
        case VirtualKeyCode::Numpad2: return "Numpad2";
        case VirtualKeyCode::Numpad3: return "Numpad3";
        case VirtualKeyCode::Numpad4: return "Numpad4";
        case VirtualKeyCode::Numpad5: return "Numpad5";
        case VirtualKeyCode::Numpad6: return "Numpad6";
        case VirtualKeyCode::Numpad7: return "Numpad7";
        case VirtualKeyCode::Numpad8: return "Numpad8";
        case VirtualKeyCode::Numpad9: return "Numpad9";

        case VirtualKeyCode::AbntC1: return "AbntC1";
        case VirtualKeyCode::AbntC2: return "AbntC2";
        case VirtualKeyCode::Add: return "Add";
        case VirtualKeyCode::Apostrophe: return "Apostrophe";
        case VirtualKeyCode::Apps: return "Apps";
        case VirtualKeyCode::At: return "At";
        case VirtualKeyCode::Ax: return "Ax";
        case VirtualKeyCode::Backslash: return "Backslash";
        case VirtualKeyCode::Calculator: return "Calculator";
        case VirtualKeyCode::Capital: return "Capital";
        case VirtualKeyCode::Colon: return "Colon";
        case VirtualKeyCode::Comma: return "Comma";
        case VirtualKeyCode::Convert: return "Convert";
        case VirtualKeyCode::Decimal: return "Decimal";
        case VirtualKeyCode::Divide: return "Divide";
        case VirtualKeyCode::Equals: return "Equals";
        case VirtualKeyCode::Grave: return "Grave";
        case VirtualKeyCode::Kana: return "Kana";
        case VirtualKeyCode::Kanji: return "Kanji";
        case VirtualKeyCode::LAlt: return "LAlt";
        case VirtualKeyCode::LBracket: return "LBracket";
        case VirtualKeyCode::LControl: return "LControl";
        case VirtualKeyCode::LMenu: return "LMenu";
        case VirtualKeyCode::LShift: return "LShift";
        case VirtualKeyCode::LWin: return "LWin";
        case VirtualKeyCode::Mail: return "Mail";
        case VirtualKeyCode::MediaSelect: return "MediaSelect";
        case VirtualKeyCode::MediaStop: return "MediaStop";
        case VirtualKeyCode::Minus: return "Minus";
        case VirtualKeyCode::Multiply: return "Multiply";
        case VirtualKeyCode::Mute: return "Mute";
        case VirtualKeyCode::MyComputer: return "MyComputer";
        case VirtualKeyCode::NavigateForward: return "NavigateForward";
        case VirtualKeyCode::NavigateBackward: return "NavigateBackward";
        case VirtualKeyCode::NextTrack: return "NextTrack";
        case VirtualKeyCode::NoConvert: return "NoConvert";
        case VirtualKeyCode::NumpadComma: return "NumpadComma";
        case VirtualKeyCode::NumpadEnter: return "NumpadEnter";
        case VirtualKeyCode::NumpadEquals: return "NumpadEquals";
        case VirtualKeyCode::OEM102: return "OEM102";
        case VirtualKeyCode::Period: return "Period";
        case VirtualKeyCode::PlayPause: return "PlayPause";
        case VirtualKeyCode::Power: return "Power";
        case VirtualKeyCode::PrevTrack: return "PrevTrack";
        case VirtualKeyCode::RAlt: return "RAlt";
        case VirtualKeyCode::RBracket: return "RBracket";
        case VirtualKeyCode::RControl: return "RControl";
        case VirtualKeyCode::RMenu: return "RMenu";
        case VirtualKeyCode::RShift: return "RShift";
        case VirtualKeyCode::RWin: return "RWin";
        case VirtualKeyCode::Semicolon: return "Semicolon";
        case VirtualKeyCode::Slash: return "Slash";
        case VirtualKeyCode::Sleep: return "Sleep";
        case VirtualKeyCode::Stop: return "Stop";
        case VirtualKeyCode::Subtract: return "Subtract";
        case VirtualKeyCode::Sysrq: return "Sysrq";
        case VirtualKeyCode::Tab: return "Tab";
        case VirtualKeyCode::Underline: return "Underline";
        case VirtualKeyCode::Unlabeled: return "Unlabeled";
        case VirtualKeyCode::VolumeDown: return "VolumeDown";
        case VirtualKeyCode::VolumeUp: return "VolumeUp";
        case VirtualKeyCode::Wake: return "Wake";
        case VirtualKeyCode::WebBack: return "WebBack";
        case VirtualKeyCode::WebFavorites: return "WebFavorites";
        case VirtualKeyCode::WebForward: return "WebForward";
        case VirtualKeyCode::WebHome: return "WebHome";
        case VirtualKeyCode::WebRefresh: return "WebRefresh";
        case VirtualKeyCode::WebSearch: return "WebSearch";
        case VirtualKeyCode::WebStop: return "WebStop";
        case VirtualKeyCode::Yen: return "Yen";

        default:
            unreachable();
    }
}

inline std::optional<VirtualKeyCode> ParseVirtualKeyCode(const String &name)
{
    BEGIN_STRING_TO_ENUM(name);
    RETURN_STRING_ENUM(name, "Key0", VirtualKeyCode::Key0);
    RETURN_STRING_ENUM(name, "Key1", VirtualKeyCode::Key1);
    RETURN_STRING_ENUM(name, "Key2", VirtualKeyCode::Key2);
    RETURN_STRING_ENUM(name, "Key3", VirtualKeyCode::Key3);
    RETURN_STRING_ENUM(name, "Key4", VirtualKeyCode::Key4);
    RETURN_STRING_ENUM(name, "Key5", VirtualKeyCode::Key5);
    RETURN_STRING_ENUM(name, "Key6", VirtualKeyCode::Key6);
    RETURN_STRING_ENUM(name, "Key7", VirtualKeyCode::Key7);
    RETURN_STRING_ENUM(name, "Key8", VirtualKeyCode::Key8);
    RETURN_STRING_ENUM(name, "Key9", VirtualKeyCode::Key9);

    RETURN_STRING_ENUM(name, "A", VirtualKeyCode::A);
    RETURN_STRING_ENUM(name, "B", VirtualKeyCode::B);
    RETURN_STRING_ENUM(name, "C", VirtualKeyCode::C);
    RETURN_STRING_ENUM(name, "D", VirtualKeyCode::D);
    RETURN_STRING_ENUM(name, "E", VirtualKeyCode::E);
    RETURN_STRING_ENUM(name, "F", VirtualKeyCode::F);
    RETURN_STRING_ENUM(name, "G", VirtualKeyCode::G);
    RETURN_STRING_ENUM(name, "H", VirtualKeyCode::H);
    RETURN_STRING_ENUM(name, "I", VirtualKeyCode::I);
    RETURN_STRING_ENUM(name, "J", VirtualKeyCode::J);
    RETURN_STRING_ENUM(name, "K", VirtualKeyCode::K);
    RETURN_STRING_ENUM(name, "L", VirtualKeyCode::L);
    RETURN_STRING_ENUM(name, "M", VirtualKeyCode::M);
    RETURN_STRING_ENUM(name, "N", VirtualKeyCode::N);
    RETURN_STRING_ENUM(name, "O", VirtualKeyCode::O);
    RETURN_STRING_ENUM(name, "P", VirtualKeyCode::P);
    RETURN_STRING_ENUM(name, "Q", VirtualKeyCode::Q);
    RETURN_STRING_ENUM(name, "R", VirtualKeyCode::R);
    RETURN_STRING_ENUM(name, "S", VirtualKeyCode::S);
    RETURN_STRING_ENUM(name, "T", VirtualKeyCode::T);
    RETURN_STRING_ENUM(name, "U", VirtualKeyCode::U);
    RETURN_STRING_ENUM(name, "V", VirtualKeyCode::V);
    RETURN_STRING_ENUM(name, "W", VirtualKeyCode::W);
    RETURN_STRING_ENUM(name, "X", VirtualKeyCode::X);
    RETURN_STRING_ENUM(name, "Y", VirtualKeyCode::Y);
    RETURN_STRING_ENUM(name, "Z", VirtualKeyCode::Z);

    RETURN_STRING_ENUM(name, "Escape", VirtualKeyCode::Escape);

    RETURN_STRING_ENUM(name, "F1", VirtualKeyCode::F1);
    RETURN_STRING_ENUM(name, "F2", VirtualKeyCode::F2);
    RETURN_STRING_ENUM(name, "F3", VirtualKeyCode::F3);
    RETURN_STRING_ENUM(name, "F4", VirtualKeyCode::F4);
    RETURN_STRING_ENUM(name, "F5", VirtualKeyCode::F5);
    RETURN_STRING_ENUM(name, "F6", VirtualKeyCode::F6);
    RETURN_STRING_ENUM(name, "F7", VirtualKeyCode::F7);
    RETURN_STRING_ENUM(name, "F8", VirtualKeyCode::F8);
    RETURN_STRING_ENUM(name, "F9", VirtualKeyCode::F9);
    RETURN_STRING_ENUM(name, "F10", VirtualKeyCode::F10);
    RETURN_STRING_ENUM(name, "F11", VirtualKeyCode::F11);
    RETURN_STRING_ENUM(name, "F12", VirtualKeyCode::F12);
    RETURN_STRING_ENUM(name, "F13", VirtualKeyCode::F13);
    RETURN_STRING_ENUM(name, "F14", VirtualKeyCode::F14);
    RETURN_STRING_ENUM(name, "F15", VirtualKeyCode::F15);

    RETURN_STRING_ENUM(name, "Snapshot", VirtualKeyCode::Snapshot);
    RETURN_STRING_ENUM(name, "Scroll", VirtualKeyCode::Scroll);
    RETURN_STRING_ENUM(name, "Pause", VirtualKeyCode::Pause);

    RETURN_STRING_ENUM(name, "Insert", VirtualKeyCode::Insert);
    RETURN_STRING_ENUM(name, "Home", VirtualKeyCode::Home);
    RETURN_STRING_ENUM(name, "Delete", VirtualKeyCode::Delete);
    RETURN_STRING_ENUM(name, "End", VirtualKeyCode::End);
    RETURN_STRING_ENUM(name, "PageDown", VirtualKeyCode::PageDown);
    RETURN_STRING_ENUM(name, "PageUp", VirtualKeyCode::PageUp);

    RETURN_STRING_ENUM(name, "Left", VirtualKeyCode::Left);
    RETURN_STRING_ENUM(name, "Up", VirtualKeyCode::Up);
    RETURN_STRING_ENUM(name, "Right", VirtualKeyCode::Right);
    RETURN_STRING_ENUM(name, "Down", VirtualKeyCode::Down);

    RETURN_STRING_ENUM(name, "Back", VirtualKeyCode::Back);
    RETURN_STRING_ENUM(name, "Return", VirtualKeyCode::Return);
    RETURN_STRING_ENUM(name, "Space", VirtualKeyCode::Space);

    RETURN_STRING_ENUM(name, "Numlock", VirtualKeyCode::Numlock);
    RETURN_STRING_ENUM(name, "Numpad0", VirtualKeyCode::Numpad0);
    RETURN_STRING_ENUM(name, "Numpad1", VirtualKeyCode::Numpad1);
    RETURN_STRING_ENUM(name, "Numpad2", VirtualKeyCode::Numpad2);
    RETURN_STRING_ENUM(name, "Numpad3", VirtualKeyCode::Numpad3);
    RETURN_STRING_ENUM(name, "Numpad4", VirtualKeyCode::Numpad4);
    RETURN_STRING_ENUM(name, "Numpad5", VirtualKeyCode::Numpad5);
    RETURN_STRING_ENUM(name, "Numpad6", VirtualKeyCode::Numpad6);
    RETURN_STRING_ENUM(name, "Numpad7", VirtualKeyCode::Numpad7);
    RETURN_STRING_ENUM(name, "Numpad8", VirtualKeyCode::Numpad8);
    RETURN_STRING_ENUM(name, "Numpad9", VirtualKeyCode::Numpad9);

    RETURN_STRING_ENUM(name, "AbntC1", VirtualKeyCode::AbntC1);
    RETURN_STRING_ENUM(name, "AbntC2", VirtualKeyCode::AbntC2);
    RETURN_STRING_ENUM(name, "Add", VirtualKeyCode::Add);
    RETURN_STRING_ENUM(name, "Apostrophe", VirtualKeyCode::Apostrophe);
    RETURN_STRING_ENUM(name, "Apps", VirtualKeyCode::Apps);
    RETURN_STRING_ENUM(name, "At", VirtualKeyCode::At);
    RETURN_STRING_ENUM(name, "Ax", VirtualKeyCode::Ax);
    RETURN_STRING_ENUM(name, "Backslash", VirtualKeyCode::Backslash);
    RETURN_STRING_ENUM(name, "Calculator", VirtualKeyCode::Calculator);
    RETURN_STRING_ENUM(name, "Capital", VirtualKeyCode::Capital);
    RETURN_STRING_ENUM(name, "Colon", VirtualKeyCode::Colon);
    RETURN_STRING_ENUM(name, "Comma", VirtualKeyCode::Comma);
    RETURN_STRING_ENUM(name, "Convert", VirtualKeyCode::Convert);
    RETURN_STRING_ENUM(name, "Decimal", VirtualKeyCode::Decimal);
    RETURN_STRING_ENUM(name, "Divide", VirtualKeyCode::Divide);
    RETURN_STRING_ENUM(name, "Equals", VirtualKeyCode::Equals);
    RETURN_STRING_ENUM(name, "Grave", VirtualKeyCode::Grave);
    RETURN_STRING_ENUM(name, "Kana", VirtualKeyCode::Kana);
    RETURN_STRING_ENUM(name, "Kanji", VirtualKeyCode::Kanji);
    RETURN_STRING_ENUM(name, "LAlt", VirtualKeyCode::LAlt);
    RETURN_STRING_ENUM(name, "LBracket", VirtualKeyCode::LBracket);
    RETURN_STRING_ENUM(name, "LControl", VirtualKeyCode::LControl);
    RETURN_STRING_ENUM(name, "LMenu", VirtualKeyCode::LMenu);
    RETURN_STRING_ENUM(name, "LShift", VirtualKeyCode::LShift);
    RETURN_STRING_ENUM(name, "LWin", VirtualKeyCode::LWin);
    RETURN_STRING_ENUM(name, "Mail", VirtualKeyCode::Mail);
    RETURN_STRING_ENUM(name, "MediaSelect", VirtualKeyCode::MediaSelect);
    RETURN_STRING_ENUM(name, "MediaStop", VirtualKeyCode::MediaStop);
    RETURN_STRING_ENUM(name, "Minus", VirtualKeyCode::Minus);
    RETURN_STRING_ENUM(name, "Multiply", VirtualKeyCode::Multiply);
    RETURN_STRING_ENUM(name, "Mute", VirtualKeyCode::Mute);
    RETURN_STRING_ENUM(name, "MyComputer", VirtualKeyCode::MyComputer);
    RETURN_STRING_ENUM(name, "NavigateForward", VirtualKeyCode::NavigateForward);
    RETURN_STRING_ENUM(name, "NavigateBackward", VirtualKeyCode::NavigateBackward);
    RETURN_STRING_ENUM(name, "NextTrack", VirtualKeyCode::NextTrack);
    RETURN_STRING_ENUM(name, "NoConvert", VirtualKeyCode::NoConvert);
    RETURN_STRING_ENUM(name, "NumpadComma", VirtualKeyCode::NumpadComma);
    RETURN_STRING_ENUM(name, "NumpadEnter", VirtualKeyCode::NumpadEnter);
    RETURN_STRING_ENUM(name, "NumpadEquals", VirtualKeyCode::NumpadEquals);
    RETURN_STRING_ENUM(name, "OEM102", VirtualKeyCode::OEM102);
    RETURN_STRING_ENUM(name, "Period", VirtualKeyCode::Period);
    RETURN_STRING_ENUM(name, "PlayPause", VirtualKeyCode::PlayPause);
    RETURN_STRING_ENUM(name, "Power", VirtualKeyCode::Power);
    RETURN_STRING_ENUM(name, "PrevTrack", VirtualKeyCode::PrevTrack);
    RETURN_STRING_ENUM(name, "RAlt", VirtualKeyCode::RAlt);
    RETURN_STRING_ENUM(name, "RBracket", VirtualKeyCode::RBracket);
    RETURN_STRING_ENUM(name, "RControl", VirtualKeyCode::RControl);
    RETURN_STRING_ENUM(name, "RMenu", VirtualKeyCode::RMenu);
    RETURN_STRING_ENUM(name, "RShift", VirtualKeyCode::RShift);
    RETURN_STRING_ENUM(name, "RWin", VirtualKeyCode::RWin);
    RETURN_STRING_ENUM(name, "Semicolon", VirtualKeyCode::Semicolon);
    RETURN_STRING_ENUM(name, "Slash", VirtualKeyCode::Slash);
    RETURN_STRING_ENUM(name, "Sleep", VirtualKeyCode::Sleep);
    RETURN_STRING_ENUM(name, "Stop", VirtualKeyCode::Stop);
    RETURN_STRING_ENUM(name, "Subtract", VirtualKeyCode::Subtract);
    RETURN_STRING_ENUM(name, "Sysrq", VirtualKeyCode::Sysrq);
    RETURN_STRING_ENUM(name, "Tab", VirtualKeyCode::Tab);
    RETURN_STRING_ENUM(name, "Underline", VirtualKeyCode::Underline);
    RETURN_STRING_ENUM(name, "Unlabeled", VirtualKeyCode::Unlabeled);
    RETURN_STRING_ENUM(name, "VolumeDown", VirtualKeyCode::VolumeDown);
    RETURN_STRING_ENUM(name, "VolumeUp", VirtualKeyCode::VolumeUp);
    RETURN_STRING_ENUM(name, "Wake", VirtualKeyCode::Wake);
    RETURN_STRING_ENUM(name, "WebBack", VirtualKeyCode::WebBack);
    RETURN_STRING_ENUM(name, "WebFavorites", VirtualKeyCode::WebFavorites);
    RETURN_STRING_ENUM(name, "WebForward", VirtualKeyCode::WebForward);
    RETURN_STRING_ENUM(name, "WebHome", VirtualKeyCode::WebHome);
    RETURN_STRING_ENUM(name, "WebRefresh", VirtualKeyCode::WebRefresh);
    RETURN_STRING_ENUM(name, "WebSearch", VirtualKeyCode::WebSearch);
    RETURN_STRING_ENUM(name, "WebStop", VirtualKeyCode::WebStop);
    RETURN_STRING_ENUM(name, "Yen", VirtualKeyCode::Yen);
    END_STRING_TO_ENUM();
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif
