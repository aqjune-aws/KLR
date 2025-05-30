// Auto-generated by Tools/build/generate_token.py
// Modified for use in NKI

#undef TILDE /* Prevent clash of our definition with system macro. Ex AIX,     \
                ioctl.h */

#define ENDMARKER 0
#define NAME 1
#define NUMBER 2
#define STRING 3
#define NEWLINE 4
#define INDENT 5
#define DEDENT 6
#define LPAR 7
#define RPAR 8
#define LSQB 9
#define RSQB 10
#define COLON 11
#define COMMA 12
#define SEMI 13
#define PLUS 14
#define MINUS 15
#define STAR 16
#define SLASH 17
#define VBAR 18
#define AMPER 19
#define LESS 20
#define GREATER 21
#define EQUAL 22
#define DOT 23
#define PERCENT 24
#define LBRACE 25
#define RBRACE 26
#define EQEQUAL 27
#define NOTEQUAL 28
#define LESSEQUAL 29
#define GREATEREQUAL 30
#define TILDE 31
#define CIRCUMFLEX 32
#define LEFTSHIFT 33
#define RIGHTSHIFT 34
#define DOUBLESTAR 35
#define PLUSEQUAL 36
#define MINEQUAL 37
#define STAREQUAL 38
#define SLASHEQUAL 39
#define PERCENTEQUAL 40
#define AMPEREQUAL 41
#define VBAREQUAL 42
#define CIRCUMFLEXEQUAL 43
#define LEFTSHIFTEQUAL 44
#define RIGHTSHIFTEQUAL 45
#define DOUBLESTAREQUAL 46
#define DOUBLESLASH 47
#define DOUBLESLASHEQUAL 48
#define AT 49
#define ATEQUAL 50
#define RARROW 51
#define ELLIPSIS 52
#define COLONEQUAL 53
#define EXCLAMATION 54
#define OP 55
#define AWAIT 56
#define ASYNC 57
#define TYPE_IGNORE 58
#define TYPE_COMMENT 59
#define SOFT_KEYWORD 60
#define FSTRING_START 61
#define FSTRING_MIDDLE 62
#define FSTRING_END 63
#define COMMENT 64
#define NL 65
#define ERRORTOKEN 66
#define N_TOKENS 68
#define NT_OFFSET 256

/* Special definitions for cooperation with parser */

#define ISTERMINAL(x) ((x) < NT_OFFSET)
#define ISNONTERMINAL(x) ((x) >= NT_OFFSET)
#define ISEOF(x) ((x) == ENDMARKER)
#define ISWHITESPACE(x)                                                        \
  ((x) == ENDMARKER || (x) == NEWLINE || (x) == INDENT || (x) == DEDENT)
#define ISSTRINGLIT(x) ((x) == STRING || (x) == FSTRING_MIDDLE)

/* Token names */

static const char *const _PyParser_TokenNames[] = {
    "ENDMARKER",
    "NAME",
    "NUMBER",
    "STRING",
    "NEWLINE",
    "INDENT",
    "DEDENT",
    "LPAR",
    "RPAR",
    "LSQB",
    "RSQB",
    "COLON",
    "COMMA",
    "SEMI",
    "PLUS",
    "MINUS",
    "STAR",
    "SLASH",
    "VBAR",
    "AMPER",
    "LESS",
    "GREATER",
    "EQUAL",
    "DOT",
    "PERCENT",
    "LBRACE",
    "RBRACE",
    "EQEQUAL",
    "NOTEQUAL",
    "LESSEQUAL",
    "GREATEREQUAL",
    "TILDE",
    "CIRCUMFLEX",
    "LEFTSHIFT",
    "RIGHTSHIFT",
    "DOUBLESTAR",
    "PLUSEQUAL",
    "MINEQUAL",
    "STAREQUAL",
    "SLASHEQUAL",
    "PERCENTEQUAL",
    "AMPEREQUAL",
    "VBAREQUAL",
    "CIRCUMFLEXEQUAL",
    "LEFTSHIFTEQUAL",
    "RIGHTSHIFTEQUAL",
    "DOUBLESTAREQUAL",
    "DOUBLESLASH",
    "DOUBLESLASHEQUAL",
    "AT",
    "ATEQUAL",
    "RARROW",
    "ELLIPSIS",
    "COLONEQUAL",
    "EXCLAMATION",
    "OP",
    "AWAIT",
    "ASYNC",
    "TYPE_IGNORE",
    "TYPE_COMMENT",
    "SOFT_KEYWORD",
    "FSTRING_START",
    "FSTRING_MIDDLE",
    "FSTRING_END",
    "COMMENT",
    "NL",
    "<ERRORTOKEN>",
    "<ENCODING>",
    "<N_TOKENS>",
};

/* Return the token corresponding to a single character */

static int _PyToken_OneChar(int c1) {
  switch (c1) {
  case '!':
    return EXCLAMATION;
  case '%':
    return PERCENT;
  case '&':
    return AMPER;
  case '(':
    return LPAR;
  case ')':
    return RPAR;
  case '*':
    return STAR;
  case '+':
    return PLUS;
  case ',':
    return COMMA;
  case '-':
    return MINUS;
  case '.':
    return DOT;
  case '/':
    return SLASH;
  case ':':
    return COLON;
  case ';':
    return SEMI;
  case '<':
    return LESS;
  case '=':
    return EQUAL;
  case '>':
    return GREATER;
  case '@':
    return AT;
  case '[':
    return LSQB;
  case ']':
    return RSQB;
  case '^':
    return CIRCUMFLEX;
  case '{':
    return LBRACE;
  case '|':
    return VBAR;
  case '}':
    return RBRACE;
  case '~':
    return TILDE;
  }
  return OP;
}

static int _PyToken_TwoChars(int c1, int c2) {
  switch (c1) {
  case '!':
    switch (c2) {
    case '=':
      return NOTEQUAL;
    }
    break;
  case '%':
    switch (c2) {
    case '=':
      return PERCENTEQUAL;
    }
    break;
  case '&':
    switch (c2) {
    case '=':
      return AMPEREQUAL;
    }
    break;
  case '*':
    switch (c2) {
    case '*':
      return DOUBLESTAR;
    case '=':
      return STAREQUAL;
    }
    break;
  case '+':
    switch (c2) {
    case '=':
      return PLUSEQUAL;
    }
    break;
  case '-':
    switch (c2) {
    case '=':
      return MINEQUAL;
    case '>':
      return RARROW;
    }
    break;
  case '/':
    switch (c2) {
    case '/':
      return DOUBLESLASH;
    case '=':
      return SLASHEQUAL;
    }
    break;
  case ':':
    switch (c2) {
    case '=':
      return COLONEQUAL;
    }
    break;
  case '<':
    switch (c2) {
    case '<':
      return LEFTSHIFT;
    case '=':
      return LESSEQUAL;
    case '>':
      return NOTEQUAL;
    }
    break;
  case '=':
    switch (c2) {
    case '=':
      return EQEQUAL;
    }
    break;
  case '>':
    switch (c2) {
    case '=':
      return GREATEREQUAL;
    case '>':
      return RIGHTSHIFT;
    }
    break;
  case '@':
    switch (c2) {
    case '=':
      return ATEQUAL;
    }
    break;
  case '^':
    switch (c2) {
    case '=':
      return CIRCUMFLEXEQUAL;
    }
    break;
  case '|':
    switch (c2) {
    case '=':
      return VBAREQUAL;
    }
    break;
  }
  return OP;
}

static int _PyToken_ThreeChars(int c1, int c2, int c3) {
  switch (c1) {
  case '*':
    switch (c2) {
    case '*':
      switch (c3) {
      case '=':
        return DOUBLESTAREQUAL;
      }
      break;
    }
    break;
  case '.':
    switch (c2) {
    case '.':
      switch (c3) {
      case '.':
        return ELLIPSIS;
      }
      break;
    }
    break;
  case '/':
    switch (c2) {
    case '/':
      switch (c3) {
      case '=':
        return DOUBLESLASHEQUAL;
      }
      break;
    }
    break;
  case '<':
    switch (c2) {
    case '<':
      switch (c3) {
      case '=':
        return LEFTSHIFTEQUAL;
      }
      break;
    }
    break;
  case '>':
    switch (c2) {
    case '>':
      switch (c3) {
      case '=':
        return RIGHTSHIFTEQUAL;
      }
      break;
    }
    break;
  }
  return OP;
}
