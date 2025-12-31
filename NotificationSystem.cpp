#include<bits/stdc++.h>
using namespace std;

class INotification {
public:
    virtual string getContent() const = 0;
};

class SimpleNotification: public INotification {
private: 
    string text;
public:
    SimpleNotification(const string &text) {
        this->text = text;
    }
    string getContent() const override {
        return text;
    }
};
class INotificationDecorator:public INotification {
protected:
    INotification* notify;
public: 
    INotificationDecorator(INotification* n) {
        this->notify = n;
    }
    virtual ~INotificationDecorator() {
        delete notify;
    }
};
class TimeStampDecorator: public INotificationDecorator {
public:  
    TimeStampDecorator(INotification* n) : INotificationDecorator(n) { }
    string getContent() const override {
        return  "[2025-04-13 14:22:00] " + notify->getContent();
    }
};
class SignatureDecorator: public INotificationDecorator {
    private:
    string signature;
    public: 
    SignatureDecorator(INotification* n, const string& sig) : INotificationDecorator(n) {
        signature = sig;
     }
    string getContent() const override {
        return notify->getContent() + "\n-- " + signature + "\n\n";
    }
};

class IObserver {
public:
   virtual void update() const = 0;
   virtual ~ IObserver() {}
};

class IObservable {
    public:
    virtual void addObserver(IObserver* o)  = 0;
    virtual void removeObserver(IObserver* r) = 0;
    virtual void notifyObservers() = 0;
};

  
class NotificationObservable:public IObservable {
private:
    vector<IObserver*> observers;
    INotification* currNotification;
public:
    NotificationObservable() {
        currNotification = nullptr;
    }
    void addObserver(IObserver* obs) override {
        observers.push_back(obs);   
    }
    void removeObserver(IObserver* r) override {
        observers.erase(remove(observers.begin(), observers.end(), r), observers.end());
    }

    void notifyObservers() override {
        for(unsigned int i=0; i<observers.size(); i++) {
            observers[i]->update();
        }
    }
    void setNotification(INotification* n) {
        if(currNotification != nullptr) {
            delete currNotification;
        }
        currNotification = n;
        notifyObservers();
    }
    INotification* getNotification() {
        return currNotification;
    }

    string getNotificationContent() {
        return currNotification->getContent();
    }
    ~NotificationObservable() {
        if(currNotification != NULL) {
            delete currNotification;
        }
    }
};

