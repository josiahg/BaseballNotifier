/* TODO
 * - Pull down data in a stream, pass directly, no more file save/delete operations
 * - Custom notification icon?  
 * - More robust error handling/input sanitization
 */
#include <libnotifymm-1.0/libnotifymm.h>
#include <tinyxml.h>
#include <string>
#include <iostream>
#include <vector>
#include <ctime>
#include <unistd.h>

// Will use these in a future version
//#include <curlpp/cURLpp.hpp>
//#include <curlpp/Options.hpp>
//#include <curlpp/Easy.hpp>

struct Game
{
    std::string id = "";
    std::string status = "";
    std::string teamName1 = "";
    std::string teamName2 = "";
    std::string score1 = "";
    std::string score2 = "";
};

std::vector<Game> games;

void showNotification(std::string subject, std::string message)
{
    try
    {
        Notify::init("MLB Notifier");
        Notify::Notification n(subject, message, "dialog-information");
        n.set_timeout(5000);
        n.show();
    } 
    catch(std::exception e)
    {
        std::cerr << "Could not display notification.";
    }
}

std::string urlGenerator()
{
    // generate the date pieces we need
    time_t t = time(0);
    struct tm* now = localtime(&t);
    std::string year = std::to_string(now->tm_year + 1900);
    std::string month = std::to_string(now->tm_mon + 1);
    std::string day = std::to_string(now->tm_mday);
    if(month.length() < 2)
        month = "0" + month;
    if(day.length() < 2)
        day = "0" + day;
    
    std::string url_base = "http://gd2.mlb.com/components/game/mlb/year_";
    std::string url = url_base + year + "/month_" + month + "/day_" + day + "/scoreboard.xml";
   
    return url;
}

void checkDiff(Game* oldgame, Game* newgame)
{
    std::string subject = "";
    std::string message = "";
    if(oldgame->status != newgame->status)
    {
        subject = newgame->status;
        message = newgame->teamName1 + " " + newgame->score1 + " " + newgame->teamName2 + " "
            + newgame->score2;
        showNotification(subject, message);
    }
    if((oldgame->score1 != newgame->score1 || oldgame->score2 != newgame->score2))
    {
        std::string subject = "Score Update";
        message = newgame->teamName1 + " " + newgame->score1 + " " + newgame->teamName2 + " "
            + newgame->score2;
        showNotification(subject, message);
    }
        
}

void loadGames(const char* fName)
{
    TiXmlDocument doc(fName);
    
    if(doc.LoadFile())
    {
        TiXmlElement* scoreboard = doc.FirstChildElement();
        if(scoreboard)
        {
            Game curr_game;
            TiXmlElement* game = scoreboard->FirstChildElement();
            while(game)
            {
                TiXmlElement* subgame = game->FirstChildElement("game");
                
                std::string id = subgame->Attribute("id");
                curr_game.id = id;

                std::string status = subgame->Attribute("status");
                curr_game.status = status;
                
                TiXmlElement* team1 = game->FirstChildElement("team");
                curr_game.teamName1 = team1->FirstAttribute()->Value();
                
                TiXmlElement* team2 = team1->NextSiblingElement("team");
                curr_game.teamName2 = team2->FirstAttribute()->Value();
                
                TiXmlElement* gameteam1 = team1->FirstChildElement("gameteam");
                curr_game.score1 = gameteam1->Attribute("R");

                TiXmlElement* gameteam2 = team2->FirstChildElement("gameteam");
                curr_game.score2 = gameteam2->Attribute("R");

                auto it = find_if(games.begin(), games.end(), [&id](const Game& obj) { return obj.id == id; });
                if(it != games.end())
                {
                    //found a matching game
                    // get the index
                    auto index = std::distance(games.begin(), it);
                    Game old_game = games[index];
                    checkDiff(&old_game, &curr_game);
                    games.erase(games.begin() + index);
                    games.push_back(curr_game);
                }
                else
                {
                    games.push_back(curr_game);
                }
                game = game->NextSiblingElement();
            }
        }
        else
        {
            printf("no scoreboard\n");
        }

    }
}

void printGames()
{
    for(auto &game : games)
    {
        std::string gamestring = game.status + ": " + game.teamName1 + " " + game.score1 + " - " 
            + game.teamName2 + " " + game.score2;
        std::cout << gamestring << "\n";
    }
}
void downloadGames()
{
    std::string url = urlGenerator();
    std::string command = "curl " + url + " -sS -o d_scoreboard.xml";
    system(command.c_str());
    loadGames("d_scoreboard.xml");
    system("rm d_scoreboard.xml"); 
}

int main(int argc, char* argv[])
{
    showNotification("MLB Notifier", "Up and running!");
    if(argc == 1)
    {
        while(true)
        {
             downloadGames();
             //printGames();
             sleep(180);
        }    
    }
    // The following is debug code, execute the notifier with arguments of one or two
    // scoreboard.xml files to test with those files rather than the online versions 
    if(argc > 1)
    {
        loadGames(argv[1]);
    }
    if(argc > 2)
    {
        loadGames(argv[2]);
    }
   
    return 0;
    
}
