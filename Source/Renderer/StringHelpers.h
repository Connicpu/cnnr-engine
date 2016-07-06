#pragma once

#include "Events.h"

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

inline std::optional<EventType> ParseEventType(const char *name)
{
    if (strcmp(name, "Resized") == 0) { return EventType::Resized; }
    if (strcmp(name, "Moved") == 0) { return EventType::Moved; }
    if (strcmp(name, "Closed") == 0) { return EventType::Closed; }
    if (strcmp(name, "DroppedFile") == 0) { return EventType::DroppedFile; }
    if (strcmp(name, "ReceivedCharacter") == 0) { return EventType::ReceivedCharacter; }
    if (strcmp(name, "Focused") == 0) { return EventType::Focused; }
    if (strcmp(name, "KeyboardInput") == 0) { return EventType::KeyboardInput; }
    if (strcmp(name, "MouseMoved") == 0) { return EventType::MouseMoved; }
    if (strcmp(name, "MouseWheel") == 0) { return EventType::MouseWheel; }
    if (strcmp(name, "MouseInput") == 0) { return EventType::MouseInput; }
    if (strcmp(name, "Touch") == 0) { return EventType::Touch; }

    return std::nullopt;
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

inline std::optional<TouchPhase> ParseTouchPhase(const char *name)
{
    if (strcmp(name, "Started") == 0) { return TouchPhase::Started; }
    if (strcmp(name, "Moved") == 0) { return TouchPhase::Moved; }
    if (strcmp(name, "Ended") == 0) { return TouchPhase::Ended; }
    if (strcmp(name, "Cancelled") == 0) { return TouchPhase::Cancelled; }

    return std::nullopt;
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

inline std::optional<VirtualKeyCode> ParseVirtualKeyCode(const char *name)
{
    if (strcmp(name, "Key0") == 0) { return VirtualKeyCode::Key0; }
    if (strcmp(name, "Key1") == 0) { return VirtualKeyCode::Key1; }
    if (strcmp(name, "Key2") == 0) { return VirtualKeyCode::Key2; }
    if (strcmp(name, "Key3") == 0) { return VirtualKeyCode::Key3; }
    if (strcmp(name, "Key4") == 0) { return VirtualKeyCode::Key4; }
    if (strcmp(name, "Key5") == 0) { return VirtualKeyCode::Key5; }
    if (strcmp(name, "Key6") == 0) { return VirtualKeyCode::Key6; }
    if (strcmp(name, "Key7") == 0) { return VirtualKeyCode::Key7; }
    if (strcmp(name, "Key8") == 0) { return VirtualKeyCode::Key8; }
    if (strcmp(name, "Key9") == 0) { return VirtualKeyCode::Key9; }

    if (strcmp(name, "A") == 0) { return VirtualKeyCode::A; }
    if (strcmp(name, "B") == 0) { return VirtualKeyCode::B; }
    if (strcmp(name, "C") == 0) { return VirtualKeyCode::C; }
    if (strcmp(name, "D") == 0) { return VirtualKeyCode::D; }
    if (strcmp(name, "E") == 0) { return VirtualKeyCode::E; }
    if (strcmp(name, "F") == 0) { return VirtualKeyCode::F; }
    if (strcmp(name, "G") == 0) { return VirtualKeyCode::G; }
    if (strcmp(name, "H") == 0) { return VirtualKeyCode::H; }
    if (strcmp(name, "I") == 0) { return VirtualKeyCode::I; }
    if (strcmp(name, "J") == 0) { return VirtualKeyCode::J; }
    if (strcmp(name, "K") == 0) { return VirtualKeyCode::K; }
    if (strcmp(name, "L") == 0) { return VirtualKeyCode::L; }
    if (strcmp(name, "M") == 0) { return VirtualKeyCode::M; }
    if (strcmp(name, "N") == 0) { return VirtualKeyCode::N; }
    if (strcmp(name, "O") == 0) { return VirtualKeyCode::O; }
    if (strcmp(name, "P") == 0) { return VirtualKeyCode::P; }
    if (strcmp(name, "Q") == 0) { return VirtualKeyCode::Q; }
    if (strcmp(name, "R") == 0) { return VirtualKeyCode::R; }
    if (strcmp(name, "S") == 0) { return VirtualKeyCode::S; }
    if (strcmp(name, "T") == 0) { return VirtualKeyCode::T; }
    if (strcmp(name, "U") == 0) { return VirtualKeyCode::U; }
    if (strcmp(name, "V") == 0) { return VirtualKeyCode::V; }
    if (strcmp(name, "W") == 0) { return VirtualKeyCode::W; }
    if (strcmp(name, "X") == 0) { return VirtualKeyCode::X; }
    if (strcmp(name, "Y") == 0) { return VirtualKeyCode::Y; }
    if (strcmp(name, "Z") == 0) { return VirtualKeyCode::Z; }

    if (strcmp(name, "Escape") == 0) { return VirtualKeyCode::Escape; }

    if (strcmp(name, "F1") == 0) { return VirtualKeyCode::F1; }
    if (strcmp(name, "F2") == 0) { return VirtualKeyCode::F2; }
    if (strcmp(name, "F3") == 0) { return VirtualKeyCode::F3; }
    if (strcmp(name, "F4") == 0) { return VirtualKeyCode::F4; }
    if (strcmp(name, "F5") == 0) { return VirtualKeyCode::F5; }
    if (strcmp(name, "F6") == 0) { return VirtualKeyCode::F6; }
    if (strcmp(name, "F7") == 0) { return VirtualKeyCode::F7; }
    if (strcmp(name, "F8") == 0) { return VirtualKeyCode::F8; }
    if (strcmp(name, "F9") == 0) { return VirtualKeyCode::F9; }
    if (strcmp(name, "F10") == 0) { return VirtualKeyCode::F10; }
    if (strcmp(name, "F11") == 0) { return VirtualKeyCode::F11; }
    if (strcmp(name, "F12") == 0) { return VirtualKeyCode::F12; }
    if (strcmp(name, "F13") == 0) { return VirtualKeyCode::F13; }
    if (strcmp(name, "F14") == 0) { return VirtualKeyCode::F14; }
    if (strcmp(name, "F15") == 0) { return VirtualKeyCode::F15; }

    if (strcmp(name, "Snapshot") == 0) { return VirtualKeyCode::Snapshot; }
    if (strcmp(name, "Scroll") == 0) { return VirtualKeyCode::Scroll; }
    if (strcmp(name, "Pause") == 0) { return VirtualKeyCode::Pause; }

    if (strcmp(name, "Insert") == 0) { return VirtualKeyCode::Insert; }
    if (strcmp(name, "Home") == 0) { return VirtualKeyCode::Home; }
    if (strcmp(name, "Delete") == 0) { return VirtualKeyCode::Delete; }
    if (strcmp(name, "End") == 0) { return VirtualKeyCode::End; }
    if (strcmp(name, "PageDown") == 0) { return VirtualKeyCode::PageDown; }
    if (strcmp(name, "PageUp") == 0) { return VirtualKeyCode::PageUp; }

    if (strcmp(name, "Left") == 0) { return VirtualKeyCode::Left; }
    if (strcmp(name, "Up") == 0) { return VirtualKeyCode::Up; }
    if (strcmp(name, "Right") == 0) { return VirtualKeyCode::Right; }
    if (strcmp(name, "Down") == 0) { return VirtualKeyCode::Down; }

    if (strcmp(name, "Back") == 0) { return VirtualKeyCode::Back; }
    if (strcmp(name, "Return") == 0) { return VirtualKeyCode::Return; }
    if (strcmp(name, "Space") == 0) { return VirtualKeyCode::Space; }

    if (strcmp(name, "Numlock") == 0) { return VirtualKeyCode::Numlock; }
    if (strcmp(name, "Numpad0") == 0) { return VirtualKeyCode::Numpad0; }
    if (strcmp(name, "Numpad1") == 0) { return VirtualKeyCode::Numpad1; }
    if (strcmp(name, "Numpad2") == 0) { return VirtualKeyCode::Numpad2; }
    if (strcmp(name, "Numpad3") == 0) { return VirtualKeyCode::Numpad3; }
    if (strcmp(name, "Numpad4") == 0) { return VirtualKeyCode::Numpad4; }
    if (strcmp(name, "Numpad5") == 0) { return VirtualKeyCode::Numpad5; }
    if (strcmp(name, "Numpad6") == 0) { return VirtualKeyCode::Numpad6; }
    if (strcmp(name, "Numpad7") == 0) { return VirtualKeyCode::Numpad7; }
    if (strcmp(name, "Numpad8") == 0) { return VirtualKeyCode::Numpad8; }
    if (strcmp(name, "Numpad9") == 0) { return VirtualKeyCode::Numpad9; }

    if (strcmp(name, "AbntC1") == 0) { return VirtualKeyCode::AbntC1; }
    if (strcmp(name, "AbntC2") == 0) { return VirtualKeyCode::AbntC2; }
    if (strcmp(name, "Add") == 0) { return VirtualKeyCode::Add; }
    if (strcmp(name, "Apostrophe") == 0) { return VirtualKeyCode::Apostrophe; }
    if (strcmp(name, "Apps") == 0) { return VirtualKeyCode::Apps; }
    if (strcmp(name, "At") == 0) { return VirtualKeyCode::At; }
    if (strcmp(name, "Ax") == 0) { return VirtualKeyCode::Ax; }
    if (strcmp(name, "Backslash") == 0) { return VirtualKeyCode::Backslash; }
    if (strcmp(name, "Calculator") == 0) { return VirtualKeyCode::Calculator; }
    if (strcmp(name, "Capital") == 0) { return VirtualKeyCode::Capital; }
    if (strcmp(name, "Colon") == 0) { return VirtualKeyCode::Colon; }
    if (strcmp(name, "Comma") == 0) { return VirtualKeyCode::Comma; }
    if (strcmp(name, "Convert") == 0) { return VirtualKeyCode::Convert; }
    if (strcmp(name, "Decimal") == 0) { return VirtualKeyCode::Decimal; }
    if (strcmp(name, "Divide") == 0) { return VirtualKeyCode::Divide; }
    if (strcmp(name, "Equals") == 0) { return VirtualKeyCode::Equals; }
    if (strcmp(name, "Grave") == 0) { return VirtualKeyCode::Grave; }
    if (strcmp(name, "Kana") == 0) { return VirtualKeyCode::Kana; }
    if (strcmp(name, "Kanji") == 0) { return VirtualKeyCode::Kanji; }
    if (strcmp(name, "LAlt") == 0) { return VirtualKeyCode::LAlt; }
    if (strcmp(name, "LBracket") == 0) { return VirtualKeyCode::LBracket; }
    if (strcmp(name, "LControl") == 0) { return VirtualKeyCode::LControl; }
    if (strcmp(name, "LMenu") == 0) { return VirtualKeyCode::LMenu; }
    if (strcmp(name, "LShift") == 0) { return VirtualKeyCode::LShift; }
    if (strcmp(name, "LWin") == 0) { return VirtualKeyCode::LWin; }
    if (strcmp(name, "Mail") == 0) { return VirtualKeyCode::Mail; }
    if (strcmp(name, "MediaSelect") == 0) { return VirtualKeyCode::MediaSelect; }
    if (strcmp(name, "MediaStop") == 0) { return VirtualKeyCode::MediaStop; }
    if (strcmp(name, "Minus") == 0) { return VirtualKeyCode::Minus; }
    if (strcmp(name, "Multiply") == 0) { return VirtualKeyCode::Multiply; }
    if (strcmp(name, "Mute") == 0) { return VirtualKeyCode::Mute; }
    if (strcmp(name, "MyComputer") == 0) { return VirtualKeyCode::MyComputer; }
    if (strcmp(name, "NavigateForward") == 0) { return VirtualKeyCode::NavigateForward; }
    if (strcmp(name, "NavigateBackward") == 0) { return VirtualKeyCode::NavigateBackward; }
    if (strcmp(name, "NextTrack") == 0) { return VirtualKeyCode::NextTrack; }
    if (strcmp(name, "NoConvert") == 0) { return VirtualKeyCode::NoConvert; }
    if (strcmp(name, "NumpadComma") == 0) { return VirtualKeyCode::NumpadComma; }
    if (strcmp(name, "NumpadEnter") == 0) { return VirtualKeyCode::NumpadEnter; }
    if (strcmp(name, "NumpadEquals") == 0) { return VirtualKeyCode::NumpadEquals; }
    if (strcmp(name, "OEM102") == 0) { return VirtualKeyCode::OEM102; }
    if (strcmp(name, "Period") == 0) { return VirtualKeyCode::Period; }
    if (strcmp(name, "PlayPause") == 0) { return VirtualKeyCode::PlayPause; }
    if (strcmp(name, "Power") == 0) { return VirtualKeyCode::Power; }
    if (strcmp(name, "PrevTrack") == 0) { return VirtualKeyCode::PrevTrack; }
    if (strcmp(name, "RAlt") == 0) { return VirtualKeyCode::RAlt; }
    if (strcmp(name, "RBracket") == 0) { return VirtualKeyCode::RBracket; }
    if (strcmp(name, "RControl") == 0) { return VirtualKeyCode::RControl; }
    if (strcmp(name, "RMenu") == 0) { return VirtualKeyCode::RMenu; }
    if (strcmp(name, "RShift") == 0) { return VirtualKeyCode::RShift; }
    if (strcmp(name, "RWin") == 0) { return VirtualKeyCode::RWin; }
    if (strcmp(name, "Semicolon") == 0) { return VirtualKeyCode::Semicolon; }
    if (strcmp(name, "Slash") == 0) { return VirtualKeyCode::Slash; }
    if (strcmp(name, "Sleep") == 0) { return VirtualKeyCode::Sleep; }
    if (strcmp(name, "Stop") == 0) { return VirtualKeyCode::Stop; }
    if (strcmp(name, "Subtract") == 0) { return VirtualKeyCode::Subtract; }
    if (strcmp(name, "Sysrq") == 0) { return VirtualKeyCode::Sysrq; }
    if (strcmp(name, "Tab") == 0) { return VirtualKeyCode::Tab; }
    if (strcmp(name, "Underline") == 0) { return VirtualKeyCode::Underline; }
    if (strcmp(name, "Unlabeled") == 0) { return VirtualKeyCode::Unlabeled; }
    if (strcmp(name, "VolumeDown") == 0) { return VirtualKeyCode::VolumeDown; }
    if (strcmp(name, "VolumeUp") == 0) { return VirtualKeyCode::VolumeUp; }
    if (strcmp(name, "Wake") == 0) { return VirtualKeyCode::Wake; }
    if (strcmp(name, "WebBack") == 0) { return VirtualKeyCode::WebBack; }
    if (strcmp(name, "WebFavorites") == 0) { return VirtualKeyCode::WebFavorites; }
    if (strcmp(name, "WebForward") == 0) { return VirtualKeyCode::WebForward; }
    if (strcmp(name, "WebHome") == 0) { return VirtualKeyCode::WebHome; }
    if (strcmp(name, "WebRefresh") == 0) { return VirtualKeyCode::WebRefresh; }
    if (strcmp(name, "WebSearch") == 0) { return VirtualKeyCode::WebSearch; }
    if (strcmp(name, "WebStop") == 0) { return VirtualKeyCode::WebStop; }
    if (strcmp(name, "Yen") == 0) { return VirtualKeyCode::Yen; }

    return std::nullopt;
}
