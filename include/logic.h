#pragma once
#include <nlohmann/json.hpp>
#include <string>
#include <cpr/cpr.h>

using json = nlohmann::json;

std::string GetUpdates();

extern bool IsThereUpdates;
struct Message_Keeper {

    Message_Keeper(const std::string & text_, int chat_id_) : text(text_), chat_id(chat_id_) {}
    std::string translate_text;
    std::string text;
    int chat_id;
    std::string source_lang;
    std::string target_lang;

};

struct App {
    json parser;
    explicit App(const std::string& json_ex);
    void ParseUpdates(void);
    void Process_Message(void);
    void SendTelegramMessage(const int chat_id, const std::string & text);
};
