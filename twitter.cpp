#include <iostream>
#include <vector>
#include <sstream>
#include <memory>
#include <map>
#include "auxiliar.hpp"

class Tweet {
    std::string msg;
    int id;
    std::string username;
    std::vector<std::string> likes;
    std::shared_ptr<Tweet> original;
public:
    Tweet(int id, std::string msg, std::string username) : msg(msg), id(id), username(username) {}

    int getId() {
        return id;
    }
    std::string getSender() {
        return username;
    }
    std::string getMsg() {
        return msg;
    }
    std::string toString() {
        std::stringstream ss;
        ss << "Tweet " << id << ": " << msg << " (likes: " << likes.size() << ")";
        return ss.str();
    }
    void like(std::string username) {
        likes.push_back(username);
    }
    std::vector<std::string> getLikes() {
        return likes;
    }
    void setOriginal(std::shared_ptr<Tweet> tw) {
        original = tw;
    }
    void remove() {
        msg = "";
    }
    void setDeleted() {
        msg = "esse tweet foi deletado";
        username = "";
        likes.clear();
    }
    bool isDeleted() {
        return msg == "";
    }
};

class Inbox {
    std::map<int, std::shared_ptr<Tweet>> timeline;
    std::map<int, std::shared_ptr<Tweet>> myTweets;
public:
    Inbox() {}

    void storeInTimeline(std::shared_ptr<Tweet> tweet) {
        timeline[tweet->getId()] = tweet;
    }
    std::vector<std::shared_ptr<Tweet>> getTimeline() {
        std::vector<std::shared_ptr<Tweet>> tweets;
        for (auto it = timeline.rbegin(); it != timeline.rend(); it++) {
            if (!it->second->isDeleted()) {
                tweets.push_back(it->second);
            }
        }
        return tweets;
    }
    std::string toString() {
        std::stringstream ss;
        for (auto it = timeline.rbegin(); it != timeline.rend(); it++) {
            if (!it->second->isDeleted()) {
                ss << it->second->toString() << std::endl;
            }
        }
        return ss.str();
    }
    std::shared_ptr<Tweet> getTweet(int id) {
        if (timeline.find(id) == timeline.end()) {
            throw std::runtime_error("Tweet não encontrado");
        }
        return timeline[id];
    }
    void rmMsgsFrom(std::string username) {
        for (auto it = timeline.begin(); it != timeline.end(); it++) {
            if (it->second->getSender() == username) {
                it->second->setDeleted();
            }
        }
    }
    void storeInMyTweets(std::shared_ptr<Tweet> tweet) {
        myTweets[tweet->getId()] = tweet;
    }
    std::vector<std::shared_ptr<Tweet>> getMyTweets() {
        std::vector<std::shared_ptr<Tweet>> tweets;
        for (auto it = myTweets.rbegin(); it != myTweets.rend(); it++) {
            if (!it->second->isDeleted()) {
                tweets.push_back(it->second);
            }
        }
        return tweets;
    }

};

class User {
    std::string username;
    std::vector<std::string> followers;
    std::vector<std::string> following;
    Inbox inbox;
public:
    User(std::string username) : username(username) {}

    std::string getUsername() {
        return username;
    }
    std::string getTweet() {
        return inbox.toString();
    }
    std::string getInbox() {
        return inbox.toString();
    }
    std::string toString() {
        std::stringstream ss;
        ss << "Seguidos: ";
        for (auto it = following.begin(); it != following.end(); it++) {
            ss << *it << " ";
        }
        ss << std::endl;
        ss << "Seguidores: ";
        for (auto it = followers.begin(); it != followers.end(); it++) {
            ss << *it << " ";
        }
        return ss.str();
    }
    void follow(std::string other) {
        if (other == username) {
            throw std::runtime_error("Você não pode seguir você mesmo");
        }
        if (std::find(following.begin(), following.end(), other) == following.end()) {
            following.push_back(other);
            inbox.storeInTimeline(std::make_shared<Tweet>(0, "", other));
        }
    }
    void sendTweet(std::shared_ptr<Tweet> tw) {
        inbox.storeInMyTweets(tw);
        inbox.storeInTimeline(tw);
        for (auto it = following.begin(); it != following.end(); it++) {
            inbox.storeInTimeline(std::make_shared<Tweet>(0, "", *it));
        }
    }
    void unfollow(std::string other) {
        if (std::find(following.begin(), following.end(), other) != following.end()) {
            inbox.rmMsgsFrom(other);
            for (auto it = followers.begin(); it != followers.end(); it++) {
                if (*it == other) {
                    followers.erase(it);
                    break;
                }
            }
            for (auto it = following.begin(); it != following.end(); it++) {
                if (*it == other) {
                    following.erase(it);
                    break;
                }
            }
        }
    }
    void like(int id) {
        if (inbox.getTweet(id)->isDeleted()) {
            throw std::runtime_error("Tweet não encontrado");
        }
        inbox.getTweet(id)->like(username);
    }
    void unfollowAll() {
        for (auto it = following.begin(); it != following.end(); it++) {
            inbox.rmMsgsFrom(*it);
            for (auto it2 = followers.begin(); it2 != followers.end(); it2++) {
                if (*it2 == *it) {
                    followers.erase(it2);
                    break;
                }
            }
        }
        following.clear();
    }
    void rejectAll() {
        for (auto it = followers.begin(); it != followers.end(); it++) {
            inbox.rmMsgsFrom(*it);
            for (auto it2 = following.begin(); it2 != following.end(); it2++) {
                if (*it2 == *it) {
                    following.erase(it2);
                    break;
                }
            }
        }
        followers.clear();
    }
};

class Controller {
    std::map<std::string, std::shared_ptr<User>> users;
    std::map<int, std::shared_ptr<Tweet>> tweets;
    int nextTweetId = 0;

public:
    Controller() {}

    friend std::ostream& operator<<(std::ostream& os, const Controller& ctrl);

    void addUser(std::string username) {
        if (users.find(username) != users.end()) {
            throw std::runtime_error("Usuario ja existe");
        }
        users[username] = std::make_shared<User>(username);
    }
    std::string toString() {
        std::stringstream ss;
        for (auto it = users.begin(); it != users.end(); it++) {
            ss << it->second->toString() << std::endl;
        }
        return ss.str();
    }
    std::shared_ptr<Tweet> createTweet(std::string sender, std::string msg) {
        if (users.find(sender) == users.end()) {
            throw std::runtime_error("Usuario nao existe");
        }
        std::shared_ptr<Tweet> tw = std::make_shared<Tweet>(nextTweetId, msg, sender);
        tweets[nextTweetId] = tw;
        nextTweetId++;
        return tw;
    }
    std::shared_ptr<User> getUser(std::string username) {
        if (users.find(username) == users.end()) {
            throw std::runtime_error("Usuario nao existe");
        }
        return users[username];
    }
    void sendTweet(std::string username, std::string msg) {
        std::shared_ptr<User> user = getUser(username);
        std::shared_ptr<Tweet> tw = createTweet(username, msg);
        user->sendTweet(tw);
    }
    void sendRt(std::string username, int twId, std::string rtMsg) {
        std::shared_ptr<User> user = getUser(username);
        std::shared_ptr<Tweet> tw = createTweet(username, rtMsg);
        tw->setOriginal(tweets[twId]);
        user->sendTweet(tw);
    }
    void rmUser(std::string username) {
        std::shared_ptr<User> user = getUser(username);
        user->unfollowAll();
        user->rejectAll();
        for (auto it = tweets.begin(); it != tweets.end(); it++) {
            if (it->second->getSender() == username) {
                it->second->setDeleted();
            }
        }
        users.erase(username);
    }
};

std::ostream& operator<<(std::ostream& os, const Controller& ctrl) {
     return os << aux::join(ctrl.users, "\n", aux::fn_pair_value_deref);
}

std::vector<std::string> getcmd() {
    std::string line;
    std::getline(std::cin, line);
    std::cout << "$" << line << "\n";
    return aux::split(line, ' ');
}

int main(){
    Controller sistema;

    while(true) {
        auto param = getcmd();
        auto cmd = param[0];
        try {
            if (cmd == "end") {
                break;
            } else if (cmd == "adduser") {
                sistema.addUser(param[1]);
            } else if (cmd == "send") {
                sistema.sendTweet(param[1], param[2]);
            } else if (cmd == "rt") {
                sistema.sendRt(param[1], std::stoi(param[2]), param[3]);
            } else if (cmd == "rmuser") {
                sistema.rmUser(param[1]);
            } else if (cmd == "follow") {
                sistema.getUser(param[1])->follow(param[2]);
            } else if (cmd == "unfollow") {
                sistema.getUser(param[1])->unfollow(param[2]);
            } else if (cmd == "like") {
                sistema.getUser(param[1])->like(std::stoi(param[2]));
            } else if (cmd == "unfollowall") {
                sistema.getUser(param[1])->unfollowAll();
            } else if (cmd == "rejectall") {
                sistema.getUser(param[1])->rejectAll();
            } else if (cmd == "timeline") {
                auto msgs = sistema.getUser(param[1])->getInbox();
                std::cout << aux::join_deref(msgs, "\n") << '\n';
                auto msg = aux::join(aux::slice(param, 3), " ");
                sistema.sendRt(param[1], std::stoi(param[2]), msg);
            } else if (cmd == "inbox") {
                std::cout << sistema.getUser(param[1])->getInbox() << std::endl;
            } else if (cmd == "users") {
                std::cout << sistema.toString() << std::endl;
            } else {
                std::cout << "Comando invalido\n";
            }
        } catch (std::exception& e) {
            std::cout << e.what() << '\n';
        }
    }
}

