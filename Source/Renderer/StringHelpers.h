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

inline bool ParseEventType(const char *name, EventType &result)
{
    if (strcmp(name, "Resized") == 0) { result = EventType::Resized; return true; }
    if (strcmp(name, "Moved") == 0) { result = EventType::Moved; return true; }
    if (strcmp(name, "Closed") == 0) { result = EventType::Closed; return true; }
    if (strcmp(name, "DroppedFile") == 0) { result = EventType::DroppedFile; return true; }
    if (strcmp(name, "ReceivedCharacter") == 0) { result = EventType::ReceivedCharacter; return true; }
    if (strcmp(name, "Focused") == 0) { result = EventType::Focused; return true; }
    if (strcmp(name, "KeyboardInput") == 0) { result = EventType::KeyboardInput; return true; }
    if (strcmp(name, "MouseMoved") == 0) { result = EventType::MouseMoved; return true; }
    if (strcmp(name, "MouseWheel") == 0) { result = EventType::MouseWheel; return true; }
    if (strcmp(name, "MouseInput") == 0) { result = EventType::MouseInput; return true; }
    if (strcmp(name, "Touch") == 0) { result = EventType::Touch; return true; }
    return false;
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

inline bool ParseVirtualKeyCode(const char *name, VirtualKeyCode &result)
{
    if (strcmp(name, "Key0") == 0) { result = VirtualKeyCode::Key0; return true; }
    if (strcmp(name, "Key1") == 0) { result = VirtualKeyCode::Key1; return true; }
    if (strcmp(name, "Key2") == 0) { result = VirtualKeyCode::Key2; return true; }
    if (strcmp(name, "Key3") == 0) { result = VirtualKeyCode::Key3; return true; }
    if (strcmp(name, "Key4") == 0) { result = VirtualKeyCode::Key4; return true; }
    if (strcmp(name, "Key5") == 0) { result = VirtualKeyCode::Key5; return true; }
    if (strcmp(name, "Key6") == 0) { result = VirtualKeyCode::Key6; return true; }
    if (strcmp(name, "Key7") == 0) { result = VirtualKeyCode::Key7; return true; }
    if (strcmp(name, "Key8") == 0) { result = VirtualKeyCode::Key8; return true; }
    if (strcmp(name, "Key9") == 0) { result = VirtualKeyCode::Key9; return true; }

    if (strcmp(name, "A") == 0) { result = VirtualKeyCode::A; return true; }
    if (strcmp(name, "B") == 0) { result = VirtualKeyCode::B; return true; }
    if (strcmp(name, "C") == 0) { result = VirtualKeyCode::C; return true; }
    if (strcmp(name, "D") == 0) { result = VirtualKeyCode::D; return true; }
    if (strcmp(name, "E") == 0) { result = VirtualKeyCode::E; return true; }
    if (strcmp(name, "F") == 0) { result = VirtualKeyCode::F; return true; }
    if (strcmp(name, "G") == 0) { result = VirtualKeyCode::G; return true; }
    if (strcmp(name, "H") == 0) { result = VirtualKeyCode::H; return true; }
    if (strcmp(name, "I") == 0) { result = VirtualKeyCode::I; return true; }
    if (strcmp(name, "J") == 0) { result = VirtualKeyCode::J; return true; }
    if (strcmp(name, "K") == 0) { result = VirtualKeyCode::K; return true; }
    if (strcmp(name, "L") == 0) { result = VirtualKeyCode::L; return true; }
    if (strcmp(name, "M") == 0) { result = VirtualKeyCode::M; return true; }
    if (strcmp(name, "N") == 0) { result = VirtualKeyCode::N; return true; }
    if (strcmp(name, "O") == 0) { result = VirtualKeyCode::O; return true; }
    if (strcmp(name, "P") == 0) { result = VirtualKeyCode::P; return true; }
    if (strcmp(name, "Q") == 0) { result = VirtualKeyCode::Q; return true; }
    if (strcmp(name, "R") == 0) { result = VirtualKeyCode::R; return true; }
    if (strcmp(name, "S") == 0) { result = VirtualKeyCode::S; return true; }
    if (strcmp(name, "T") == 0) { result = VirtualKeyCode::T; return true; }
    if (strcmp(name, "U") == 0) { result = VirtualKeyCode::U; return true; }
    if (strcmp(name, "V") == 0) { result = VirtualKeyCode::V; return true; }
    if (strcmp(name, "W") == 0) { result = VirtualKeyCode::W; return true; }
    if (strcmp(name, "X") == 0) { result = VirtualKeyCode::X; return true; }
    if (strcmp(name, "Y") == 0) { result = VirtualKeyCode::Y; return true; }
    if (strcmp(name, "Z") == 0) { result = VirtualKeyCode::Z; return true; }

    if (strcmp(name, "Escape") == 0) { result = VirtualKeyCode::Escape; return true; }

    if (strcmp(name, "F1") == 0) { result = VirtualKeyCode::F1; return true; }
    if (strcmp(name, "F2") == 0) { result = VirtualKeyCode::F2; return true; }
    if (strcmp(name, "F3") == 0) { result = VirtualKeyCode::F3; return true; }
    if (strcmp(name, "F4") == 0) { result = VirtualKeyCode::F4; return true; }
    if (strcmp(name, "F5") == 0) { result = VirtualKeyCode::F5; return true; }
    if (strcmp(name, "F6") == 0) { result = VirtualKeyCode::F6; return true; }
    if (strcmp(name, "F7") == 0) { result = VirtualKeyCode::F7; return true; }
    if (strcmp(name, "F8") == 0) { result = VirtualKeyCode::F8; return true; }
    if (strcmp(name, "F9") == 0) { result = VirtualKeyCode::F9; return true; }
    if (strcmp(name, "F10") == 0) { result = VirtualKeyCode::F10; return true; }
    if (strcmp(name, "F11") == 0) { result = VirtualKeyCode::F11; return true; }
    if (strcmp(name, "F12") == 0) { result = VirtualKeyCode::F12; return true; }
    if (strcmp(name, "F13") == 0) { result = VirtualKeyCode::F13; return true; }
    if (strcmp(name, "F14") == 0) { result = VirtualKeyCode::F14; return true; }
    if (strcmp(name, "F15") == 0) { result = VirtualKeyCode::F15; return true; }

    if (strcmp(name, "Snapshot") == 0) { result = VirtualKeyCode::Snapshot; return true; }
    if (strcmp(name, "Scroll") == 0) { result = VirtualKeyCode::Scroll; return true; }
    if (strcmp(name, "Pause") == 0) { result = VirtualKeyCode::Pause; return true; }

    if (strcmp(name, "Insert") == 0) { result = VirtualKeyCode::Insert; return true; }
    if (strcmp(name, "Home") == 0) { result = VirtualKeyCode::Home; return true; }
    if (strcmp(name, "Delete") == 0) { result = VirtualKeyCode::Delete; return true; }
    if (strcmp(name, "End") == 0) { result = VirtualKeyCode::End; return true; }
    if (strcmp(name, "PageDown") == 0) { result = VirtualKeyCode::PageDown; return true; }
    if (strcmp(name, "PageUp") == 0) { result = VirtualKeyCode::PageUp; return true; }

    if (strcmp(name, "Left") == 0) { result = VirtualKeyCode::Left; return true; }
    if (strcmp(name, "Up") == 0) { result = VirtualKeyCode::Up; return true; }
    if (strcmp(name, "Right") == 0) { result = VirtualKeyCode::Right; return true; }
    if (strcmp(name, "Down") == 0) { result = VirtualKeyCode::Down; return true; }

    if (strcmp(name, "Back") == 0) { result = VirtualKeyCode::Back; return true; }
    if (strcmp(name, "Return") == 0) { result = VirtualKeyCode::Return; return true; }
    if (strcmp(name, "Space") == 0) { result = VirtualKeyCode::Space; return true; }

    if (strcmp(name, "Numlock") == 0) { result = VirtualKeyCode::Numlock; return true; }
    if (strcmp(name, "Numpad0") == 0) { result = VirtualKeyCode::Numpad0; return true; }
    if (strcmp(name, "Numpad1") == 0) { result = VirtualKeyCode::Numpad1; return true; }
    if (strcmp(name, "Numpad2") == 0) { result = VirtualKeyCode::Numpad2; return true; }
    if (strcmp(name, "Numpad3") == 0) { result = VirtualKeyCode::Numpad3; return true; }
    if (strcmp(name, "Numpad4") == 0) { result = VirtualKeyCode::Numpad4; return true; }
    if (strcmp(name, "Numpad5") == 0) { result = VirtualKeyCode::Numpad5; return true; }
    if (strcmp(name, "Numpad6") == 0) { result = VirtualKeyCode::Numpad6; return true; }
    if (strcmp(name, "Numpad7") == 0) { result = VirtualKeyCode::Numpad7; return true; }
    if (strcmp(name, "Numpad8") == 0) { result = VirtualKeyCode::Numpad8; return true; }
    if (strcmp(name, "Numpad9") == 0) { result = VirtualKeyCode::Numpad9; return true; }

    if (strcmp(name, "AbntC1") == 0) { result = VirtualKeyCode::AbntC1; return true; }
    if (strcmp(name, "AbntC2") == 0) { result = VirtualKeyCode::AbntC2; return true; }
    if (strcmp(name, "Add") == 0) { result = VirtualKeyCode::Add; return true; }
    if (strcmp(name, "Apostrophe") == 0) { result = VirtualKeyCode::Apostrophe; return true; }
    if (strcmp(name, "Apps") == 0) { result = VirtualKeyCode::Apps; return true; }
    if (strcmp(name, "At") == 0) { result = VirtualKeyCode::At; return true; }
    if (strcmp(name, "Ax") == 0) { result = VirtualKeyCode::Ax; return true; }
    if (strcmp(name, "Backslash") == 0) { result = VirtualKeyCode::Backslash; return true; }
    if (strcmp(name, "Calculator") == 0) { result = VirtualKeyCode::Calculator; return true; }
    if (strcmp(name, "Capital") == 0) { result = VirtualKeyCode::Capital; return true; }
    if (strcmp(name, "Colon") == 0) { result = VirtualKeyCode::Colon; return true; }
    if (strcmp(name, "Comma") == 0) { result = VirtualKeyCode::Comma; return true; }
    if (strcmp(name, "Convert") == 0) { result = VirtualKeyCode::Convert; return true; }
    if (strcmp(name, "Decimal") == 0) { result = VirtualKeyCode::Decimal; return true; }
    if (strcmp(name, "Divide") == 0) { result = VirtualKeyCode::Divide; return true; }
    if (strcmp(name, "Equals") == 0) { result = VirtualKeyCode::Equals; return true; }
    if (strcmp(name, "Grave") == 0) { result = VirtualKeyCode::Grave; return true; }
    if (strcmp(name, "Kana") == 0) { result = VirtualKeyCode::Kana; return true; }
    if (strcmp(name, "Kanji") == 0) { result = VirtualKeyCode::Kanji; return true; }
    if (strcmp(name, "LAlt") == 0) { result = VirtualKeyCode::LAlt; return true; }
    if (strcmp(name, "LBracket") == 0) { result = VirtualKeyCode::LBracket; return true; }
    if (strcmp(name, "LControl") == 0) { result = VirtualKeyCode::LControl; return true; }
    if (strcmp(name, "LMenu") == 0) { result = VirtualKeyCode::LMenu; return true; }
    if (strcmp(name, "LShift") == 0) { result = VirtualKeyCode::LShift; return true; }
    if (strcmp(name, "LWin") == 0) { result = VirtualKeyCode::LWin; return true; }
    if (strcmp(name, "Mail") == 0) { result = VirtualKeyCode::Mail; return true; }
    if (strcmp(name, "MediaSelect") == 0) { result = VirtualKeyCode::MediaSelect; return true; }
    if (strcmp(name, "MediaStop") == 0) { result = VirtualKeyCode::MediaStop; return true; }
    if (strcmp(name, "Minus") == 0) { result = VirtualKeyCode::Minus; return true; }
    if (strcmp(name, "Multiply") == 0) { result = VirtualKeyCode::Multiply; return true; }
    if (strcmp(name, "Mute") == 0) { result = VirtualKeyCode::Mute; return true; }
    if (strcmp(name, "MyComputer") == 0) { result = VirtualKeyCode::MyComputer; return true; }
    if (strcmp(name, "NavigateForward") == 0) { result = VirtualKeyCode::NavigateForward; return true; }
    if (strcmp(name, "NavigateBackward") == 0) { result = VirtualKeyCode::NavigateBackward; return true; }
    if (strcmp(name, "NextTrack") == 0) { result = VirtualKeyCode::NextTrack; return true; }
    if (strcmp(name, "NoConvert") == 0) { result = VirtualKeyCode::NoConvert; return true; }
    if (strcmp(name, "NumpadComma") == 0) { result = VirtualKeyCode::NumpadComma; return true; }
    if (strcmp(name, "NumpadEnter") == 0) { result = VirtualKeyCode::NumpadEnter; return true; }
    if (strcmp(name, "NumpadEquals") == 0) { result = VirtualKeyCode::NumpadEquals; return true; }
    if (strcmp(name, "OEM102") == 0) { result = VirtualKeyCode::OEM102; return true; }
    if (strcmp(name, "Period") == 0) { result = VirtualKeyCode::Period; return true; }
    if (strcmp(name, "PlayPause") == 0) { result = VirtualKeyCode::PlayPause; return true; }
    if (strcmp(name, "Power") == 0) { result = VirtualKeyCode::Power; return true; }
    if (strcmp(name, "PrevTrack") == 0) { result = VirtualKeyCode::PrevTrack; return true; }
    if (strcmp(name, "RAlt") == 0) { result = VirtualKeyCode::RAlt; return true; }
    if (strcmp(name, "RBracket") == 0) { result = VirtualKeyCode::RBracket; return true; }
    if (strcmp(name, "RControl") == 0) { result = VirtualKeyCode::RControl; return true; }
    if (strcmp(name, "RMenu") == 0) { result = VirtualKeyCode::RMenu; return true; }
    if (strcmp(name, "RShift") == 0) { result = VirtualKeyCode::RShift; return true; }
    if (strcmp(name, "RWin") == 0) { result = VirtualKeyCode::RWin; return true; }
    if (strcmp(name, "Semicolon") == 0) { result = VirtualKeyCode::Semicolon; return true; }
    if (strcmp(name, "Slash") == 0) { result = VirtualKeyCode::Slash; return true; }
    if (strcmp(name, "Sleep") == 0) { result = VirtualKeyCode::Sleep; return true; }
    if (strcmp(name, "Stop") == 0) { result = VirtualKeyCode::Stop; return true; }
    if (strcmp(name, "Subtract") == 0) { result = VirtualKeyCode::Subtract; return true; }
    if (strcmp(name, "Sysrq") == 0) { result = VirtualKeyCode::Sysrq; return true; }
    if (strcmp(name, "Tab") == 0) { result = VirtualKeyCode::Tab; return true; }
    if (strcmp(name, "Underline") == 0) { result = VirtualKeyCode::Underline; return true; }
    if (strcmp(name, "Unlabeled") == 0) { result = VirtualKeyCode::Unlabeled; return true; }
    if (strcmp(name, "VolumeDown") == 0) { result = VirtualKeyCode::VolumeDown; return true; }
    if (strcmp(name, "VolumeUp") == 0) { result = VirtualKeyCode::VolumeUp; return true; }
    if (strcmp(name, "Wake") == 0) { result = VirtualKeyCode::Wake; return true; }
    if (strcmp(name, "WebBack") == 0) { result = VirtualKeyCode::WebBack; return true; }
    if (strcmp(name, "WebFavorites") == 0) { result = VirtualKeyCode::WebFavorites; return true; }
    if (strcmp(name, "WebForward") == 0) { result = VirtualKeyCode::WebForward; return true; }
    if (strcmp(name, "WebHome") == 0) { result = VirtualKeyCode::WebHome; return true; }
    if (strcmp(name, "WebRefresh") == 0) { result = VirtualKeyCode::WebRefresh; return true; }
    if (strcmp(name, "WebSearch") == 0) { result = VirtualKeyCode::WebSearch; return true; }
    if (strcmp(name, "WebStop") == 0) { result = VirtualKeyCode::WebStop; return true; }
    if (strcmp(name, "Yen") == 0) { result = VirtualKeyCode::Yen; return true; }
    return false;
}
