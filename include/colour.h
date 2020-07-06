namespace Colour {
    enum Code {
        FG_RED = 31,
        FG_GREEN = 32,
        FG_BLUE = 34,
        FG_YELLOW = 33,
        BG_YELLOW = 43,
        FG_DEFAULT = 39,
        FG_MAGENTA = 35,
        FG_CYAN = 36
    };

    class Modifier {
        Code code;
    public:
        Modifier(Code c) : code(c) {}
        friend std::ostream& operator<<(std::ostream& os, const Modifier& mod) {
            return os << "\033[" << mod.code << "m"; 
        }
    };
}
