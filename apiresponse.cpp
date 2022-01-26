#include "apiresponse.h"
#include "deserealizator.h"
#include "division.h"
#include "worker.h"
#include "room.h"

#include <Windows.h>

using std::wcout;
using std::cout;
using std::cin;
using std::endl;

APIResponse::APIResponse()
{
    roomManager = new QNetworkAccessManager;
    workerManager = new QNetworkAccessManager;
    divisionManager = new QNetworkAccessManager;

    connect(roomManager, &QNetworkAccessManager::finished, this, &APIResponse::onRoomResult);
    connect(workerManager, &QNetworkAccessManager::finished, this, &APIResponse::onWorkerResult);
    connect(divisionManager, &QNetworkAccessManager::finished, this, &APIResponse::onDivisionResult);
}

void APIResponse::onRoomResult(QNetworkReply *roomReply)
{
    MyCustomClass match;
    Deserealizator des;
    QJsonDocument document = QJsonDocument::fromJson(roomReply->readAll());
    QJsonArray rooms = document.array();

    int falseCounter = 0;
    int counter = 1;
    for(const auto& room: rooms)
    {
        auto new_room = std::make_shared<Room>();
        des.Deserealize(new_room.get(), room.toObject());

        if(match.Match(*new_room, properties, userInput)){
            qDebug() << "Room " << counter << "----> " << new_room->getWrpName() << "\n";
            ++counter;
        }else{
            ++falseCounter;
        }
        //list.append(new_slot);

    }
    if(falseCounter == rooms.count()){
        wcout << L"таких комнат нет" << endl;
    }
}

void APIResponse::onWorkerResult(QNetworkReply *workerReply)
{
    MyCustomClass match;
    Deserealizator des;
    QJsonDocument document = QJsonDocument::fromJson(workerReply->readAll());
    QJsonArray workers = document.array();

    int falseCounter = 0;
    int counter = 1;
    for(const auto& worker: workers)
    {
        auto new_worker = std::make_shared<Worker>();
        des.Deserealize(new_worker.get(), worker.toObject());
        if(match.Match(*new_worker, properties, userInput)){
            qDebug() << counter << " ---->" << new_worker->getWorFamily() << " " << new_worker->getWorName() << " "  << new_worker->getWorSurname();
            ++counter;
        }else{
            ++falseCounter;
        }
    }

    if(falseCounter == workers.count()){
        wcout << L"таких работников нет" << endl;
    }
}

void APIResponse::onDivisionResult(QNetworkReply *divisionReply)
{
    MyCustomClass match;
    Deserealizator des;
    QJsonDocument document = QJsonDocument::fromJson(divisionReply->readAll());
    QJsonArray divisions = document.array();

    int falseCounter = 0;
    int counter = 1;
    for(const auto& division: divisions)
    {
        auto new_division = std::make_shared<Division>();
        des.Deserealize(new_division.get(), division.toObject());
        if(match.Match(*new_division, properties, userInput)){
            qDebug() << counter << " ----> " << new_division->getDivName();
            ++counter;
        }else{
            ++falseCounter;
        }
        //list.append(new_slot);
    }
    if(falseCounter == divisions.count()){
        wcout << L"таких подразделений нет" << endl;
    }
}

QNetworkAccessManager *APIResponse::getDivisionManager() const
{
    return divisionManager;
}

void APIResponse::setDivisionManager(QNetworkAccessManager *newDivisionManager)
{
    divisionManager = newDivisionManager;
}

QNetworkAccessManager *APIResponse::getWorkerManager() const
{
    return workerManager;
}

void APIResponse::setWorkerManager(QNetworkAccessManager *newWorkerManager)
{
    workerManager = newWorkerManager;
}

QNetworkAccessManager *APIResponse::getRoomManager() const
{
    return roomManager;
}

void APIResponse::setRoomManager(QNetworkAccessManager *newRoomManager)
{
    roomManager = newRoomManager;
}

