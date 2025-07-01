#include "../include/logic.h"
#include "cpr/api.h"
#include "cpr/body.h"
#include "cpr/payload.h"
#include "cpr/response.h"
#include <cmath>
#include <print>
#include <vector>

const std::string BOT_TOKEN = "8048740029:AAGyo1RBvG8rLxBChQe10VTA9eXmbdaxX_Y";
const std::string YA_API_KEY = "AQVNzQmfPLpLDIZio4Njls_SeXteHchQiCOaxSLm";
const std::string Ya_url =
    "https://translate.api.cloud.yandex.net/translate/v2/translate";
const cpr::Url tg_url{"https://api.telegram.org/bot" + BOT_TOKEN};
double Last_Updates = 490094219;
std::vector<Message_Keeper> keepers;
bool IsThereUpdates = true;

App::App(const std::string &json_ex) { parser = json::parse(json_ex); }

std::string GetUpdates() {
  cpr::Url tg_url_get_updates{"https://api.telegram.org/bot" + BOT_TOKEN +
                              "/getUpdates"};
  cpr::Response response =
      cpr::Get(tg_url_get_updates,
               cpr::Parameters{{"limit", "10"},
                               {"timeout", "1"},
                               {"offset", std::to_string(Last_Updates)}});

      IsThereUpdates = true;
  return response.text;
}

void App::ParseUpdates() {
    if (parser.contains("result") && parser["result"].is_array() && !parser["result"].empty()) {
        Last_Updates = parser["result"].back()["update_id"].get<long>();
        Last_Updates++;
        IsThereUpdates = true;
    } else {
        IsThereUpdates = false;
    }

    keepers.clear();
    keepers.reserve(parser["result"].size());

  for (auto &msg : parser["result"]) {
    Message_Keeper kep = {msg["message"]["text"], msg["message"]["chat"]["id"]};
    if (msg["message"]["text"].get<std::string>().starts_with("/ru")) {
      kep.source_lang = "en";
      kep.target_lang = "ru";
    } else if (msg["message"]["text"].get<std::string>().starts_with("/en")) {
      kep.source_lang = "ru";
      kep.target_lang = "en";
    }
    else {
        continue;
    }
    kep.text = kep.text.erase(0, 4);
    keepers.push_back(std::move(kep));
  }
}

void App::Process_Message(void) {

  for (auto &kep : keepers) {
    nlohmann::json json_body{};
    json_body["sourceLanguageCode"] = kep.source_lang;
    json_body["targetLanguageCode"] = kep.target_lang;
    json_body["texts"] = {kep.text};

    cpr::Response r =
        cpr::Post(cpr::Url{Ya_url},
                  cpr::Header{{"Authorization", "Api-Key " + YA_API_KEY},
                              {"Content-Type", "application/json"}},
                  cpr::Body{json_body.dump()});
    json reply_parser = json::parse(r.text);
    kep.translate_text = reply_parser["translations"][0]["text"];
    std::println("{}", kep.translate_text);
    SendTelegramMessage(kep.chat_id, reply_parser["translations"][0]["text"]);
  }
  keepers.clear();
}

void App::SendTelegramMessage(const int chat_id,const std::string & text) {
    cpr::Payload payload{
        {"chat_id", std::to_string(chat_id)},
        {"text", text}
    };
    cpr::Response tg_response = cpr::Post(cpr::Url{tg_url + "/sendMessage"},
        payload);
}
