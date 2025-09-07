#ifndef __MCSM_ARG_PARSER_H__
#define __MCSM_ARG_PARSER_H__

#include <string>
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <functional>
#include <vector>
#include <optional>
#include <mcsm/util/string_utils.h>

namespace mcsm {
    // split args string into tokens like a shell-lite (supports quotes)
    static std::vector<std::string> tokenizeArgs(const std::string& s){
        std::vector<std::string> out;
        std::string cur;
        bool inSingle = false, inDouble = false, escaping = false;

        for(size_t i = 0; i < s.size(); i++){
            char c = s[i];

            if(escaping){
                cur.push_back(c);
                escaping = false;
                continue;
            }

            if(c == '\\'){
                escaping = true;
                continue;
            }

            if(c == '\'' && !inDouble) { inSingle = !inSingle; continue; }
            if(c == '\"' && !inSingle) { inDouble = !inDouble; continue; }

            if(std::isspace(static_cast<unsigned char>(c)) && !inSingle && !inDouble){
                if (!cur.empty()) { out.push_back(cur); cur.clear(); }
                continue;
            }

            cur.push_back(c);
        }
        if(!cur.empty()) out.push_back(cur);
        return out;
    }

    // returns {isMatch, valueIfInline}
    static std::pair<bool, std::optional<std::string>> matchOptionToken(const std::string& token, const std::string& name){
        if(token == "-" + name || token == "--" + name){
            return {true, std::nullopt}; // value may be in next token
        }

        // inline value with '='
        const std::string eq1 = "-" + name + "=";
        const std::string eq2 = "--" + name + "=";
        if(startsWith(token, eq1)) return {true, token.substr(eq1.size())};
        if(startsWith(token, eq2)) return {true, token.substr(eq2.size())};

        return {false, std::nullopt};
    }

    class ArgumentParser {
    private:
        const std::vector<std::pair<std::string, std::vector<std::string>>> options; // {base, aliases}
        std::string args;

        // For a given canonical name, collect all acceptable spellings (base + aliases)
        std::vector<std::string> namesFor(const std::string& base) const {
            std::vector<std::string> out;
            for(const auto& p : options){
                if(p.first == base){
                    out.push_back(p.first);
                    out.insert(out.end(), p.second.begin(), p.second.end());
                    break;
                }
            }
            return out;
        }

        bool isAnyOptionToken(const std::string& tok) const {
            for(const auto& p : options){
                auto names = namesFor(p.first);
                for(const auto& name : names){
                    auto [matched, inlineVal] = matchOptionToken(tok, name);
                    if (matched) return true;
                }
            }
            return false;
        }

    public:
        ArgumentParser(const std::vector<std::pair<std::string, std::vector<std::string>>>& options,
                       const std::string& args)
            : options(options), args(args) {}

        ~ArgumentParser() = default;

        inline const std::vector<std::pair<std::string, std::vector<std::string>>>& getOptions() const { return this->options; }
        inline const std::string& getArgs() const { return args; }

        inline std::string getValue(const std::string& arg) const {
            std::optional<std::string> canonical;
            for(const auto& p : options){
                if(p.first == arg) { canonical = p.first; break; }
                if(std::find(p.second.begin(), p.second.end(), arg) != p.second.end()){
                    canonical = p.first; break;
                }
            }
            if(!canonical) return "";

            const std::vector<std::string> names = namesFor(*canonical);
            const std::vector<std::string> tokens = tokenizeArgs(this->args);

            for(size_t i = 0; i < tokens.size(); i++){
                const auto& tok = tokens[i];

                for(const auto& name : names){
                    auto [matched, inlineVal] = matchOptionToken(tok, name);
                    if (!matched) continue;

                    if(inlineVal.has_value()){
                        return *inlineVal; // --opt=VALUE
                    }

                    // Accept the next token unless it is *actually* another known option token.
                    if(i + 1 < tokens.size() && !isAnyOptionToken(tokens[i + 1])){
                        return tokens[i + 1];
                    }

                    return ""; // flag present, no value
                }
            }
            return "";
        }

        inline bool flagExists(const std::string& arg) {
            std::optional<std::string> canonical;
            for(const auto& p : options){
                if(p.first == arg) { canonical = p.first; break; }
                if(std::find(p.second.begin(), p.second.end(), arg) != p.second.end()) {
                    canonical = p.first; break;
                }
            }
            if(!canonical) return false;

            const std::vector<std::string> names = namesFor(*canonical);
            const std::vector<std::string> tokens = tokenizeArgs(this->args);

            for(size_t i = 0; i < tokens.size(); i++){
                const auto& tok = tokens[i];
                for(const auto& name : names){
                    auto [matched, inlineVal] = matchOptionToken(tok, name);
                    if(matched) return true;
                }
            }
            return false;
        }
    };
}


#endif // __MCSM_ARG_PARSER_H__