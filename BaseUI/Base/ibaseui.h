#ifndef IBASEUI_H
#define IBASEUI_H


class IBaseUI{

};

// namespace IBaseUI
// {
enum DIRECT_FLAG
{
    Up2Down = 0,
    Down2Up,
    Left2Right,
    Right2Left
};

enum STATE_FLAG
{
    Normal = 0,
    Hover,
    Pressed,
    Disabled,
    Checked
};

enum STRUCT_FLAG
{
    LeftIcon = 0,
    RightIcon,
    UnderIcon,
    Upicon,
};


// }
#endif // IBASEUI_H
