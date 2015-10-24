#include "tokenizer.h"

const std::string _hexnum = "0123456789ABCDEFabcdef";

bool tokenizer::init(const char *path_)
{
        /*
         * inspired by second example from here
         * http://insanecoding.blogspot.ca/2011/11/how-to-read-in-file-in-c.html
         */
        std::ifstream is(path_);
        if (is)
        {
            is.seekg(0, std::ios::end);
            _contents.resize(is.tellg());
            is.seekg(0, std::ios::beg);
            is.read(&_contents[0], _contents.size());
            is.close();
        } else {
            return false;
        }

        _iter = _contents.begin();
        return true;
}

token tokenizer::procstr()
{
    std::string str;
    bool err = false;

    str.push_back(*_iter);
    _iter++;

    for (; _iter != _contents.end(); _iter++)
    {
        switch(*_iter) {
            case '\"':
                str.push_back(*_iter);
                _iter++;

                if (!err)
                    return token(T_STR, str);
                else
                    return token(T_ERR);
            case '\\':
                str.push_back(*_iter);
                _iter++;
                if (_iter == _contents.end())
                    return token(T_ERR);

                if (*_iter=='\"' || *_iter=='\\' || *_iter=='b' || *_iter=='f' || *_iter=='n' || *_iter=='r' || *_iter=='t') {
                    str.push_back(*_iter);
                } else if (*_iter=='u') {
                    // TODO \uXXXX
                    str.push_back(*_iter);
                } else {
                    err = true;
                }
                break;
            default:
                str.push_back(*_iter);
                break;
        }

    }

    return token(T_ERR);
}

token tokenizer::get_token()
{
    for (; _iter != _contents.end(); _iter++)
    {
        switch(*_iter) {
            case '\0':
                {
                    token tok(T_ERR);
                    _iter++;
                    return tok;
                }
            case ' ': case '\t': case '\n': case '\r':
                break; // skip whitespace
            case '{':
                {
                     token tok(T_LBRACE);
                    _iter++;
                     return tok;
                }
            case '}':
                {
                     token tok(T_RBRACE);
                    _iter++;
                     return tok;
                }
            case '[':
                {
                     token tok(T_LBRACKET);
                    _iter++;
                     return tok;
                }
            case ']':
                {
                     token tok(T_RBRACKET);
                    _iter++;
                     return tok;
                }
            case ':':
                {
                     token tok(T_COLON);
                    _iter++;
                     return tok;
                }
            case ',':
                {
                     token tok(T_COMMA);
                    _iter++;
                     return tok;
                }
            case '\"':
                {
                    return this->procstr();
                }
        }
    }

    return token(T_EOF);
}
