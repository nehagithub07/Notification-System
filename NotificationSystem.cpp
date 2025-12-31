#include <bits/stdc++.h>
using namespace std;

/* ===================== Notification Interface ===================== */
class INotification {
public:
    virtual string getContent() const = 0;
    virtual ~INotification() {}
};

/* ===================== Concrete Notification ===================== */
class SimpleNotification : public INotification {
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

/* ===================== Decorator Base ===================== */
class INotificationDecorator : public INotification {
protected:
    INotification* notify;

public:
    INotificationDecorator(INotification* n) {
        notify = n;
    }

    virtual ~INotificationDecorator() {
        delete notify;
    }
};

/* ===================== Timestamp Decorator ===================== */
class TimeStampDecorator : public INotificationDecorator {
public:
    TimeStampDecorator(INotification* n) : INotificationDecorator(n) {}

    string getContent() const override {
        return "[2025-04-13 14:22:00] " + notify->getContent();
    }
};

/* ===================== Signature Decorator ===================== */
class SignatureDecorator : public INotificationDecorator {
private:
    string signature;

public:
    SignatureDecorator(INotification* n, const string& sig)
        : INotificationDecorator(n), signature(sig) {}

    string getContent() const override {
        return notify->getContent() + "\n-- " + signature + "\n";
    }
};

/* ===================== Observer Interfaces ===================== */
class IObserver {
public:
    virtual void update() = 0;
    virtual ~IObserver() {}
};

class IObservable {
public:
    virtual void addObserver(IObserver* o) = 0;
    virtual void removeObserver(IObserver* o) = 0;
    virtual void notifyObservers() = 0;
    virtual ~IObservable() {}
};

/* ===================== Observable ===================== */
class NotificationObservable : public IObservable {
private:
    vector<IObserver*> observers;
    INotification* currNotification = nullptr;

public:
    void addObserver(IObserver* obs) override {
        observers.push_back(obs);
    }

    void removeObserver(IObserver* obs) override {
        observers.erase(remove(observers.begin(), observers.end(), obs), observers.end());
    }

    void notifyObservers() override {
        for (auto obs : observers) {
            obs->update();
        }
    }

    void setNotification(INotification* n) {
        if (currNotification != nullptr)
            delete currNotification;

        currNotification = n;
        notifyObservers();
    }

    string getNotificationContent() {
        return currNotification ? currNotification->getContent() : "";
    }

    ~NotificationObservable() {
        delete currNotification;
    }
};

/* ===================== Singleton Service ===================== */
class NotificationService {
private:
    static NotificationService* instance;
    NotificationObservable* observable;

    NotificationService() {
        observable = new NotificationObservable();
    }

public:
    static NotificationService* getInstance() {
        if (!instance)
            instance = new NotificationService();
        return instance;
    }

    NotificationObservable* getObservable() {
        return observable;
    }

    void sendNotification(INotification* notification) {
        observable->setNotification(notification);
    }

    ~NotificationService() {
        delete observable;
    }
};

NotificationService* NotificationService::instance = nullptr;

/* ===================== Logger Observer ===================== */
class Logger : public IObserver {
private:
    NotificationObservable* observable;

public:
    Logger(NotificationObservable* obs) {
        observable = obs;
        observable->addObserver(this);
    }

    void update() override {
        cout << "Logging Notification:\n"
             << observable->getNotificationContent() << endl;
    }
};

/* ===================== Strategy Interface ===================== */
class INotificationStrategy {
public:
    virtual void sendNotification(string content) = 0;
    virtual ~INotificationStrategy() {}
};

/* ===================== Email Strategy ===================== */
class EmailStrategy : public INotificationStrategy {
private:
    string emailId;

public:
    EmailStrategy(string emailId) {
        this->emailId = emailId;
    }

    void sendNotification(string content) override {
        cout << "Sending Email to: " << emailId << "\n" << content << endl;
    }
};

/* ===================== SMS Strategy ===================== */
class SMSStrategy : public INotificationStrategy {
private:
    string mobileNum;

public:
    SMSStrategy(string mobileNum) {
        this->mobileNum = mobileNum;
    }

    void sendNotification(string content) override {
        cout << "Sending SMS to: " << mobileNum << "\n" << content << endl;
    }
};

/* ===================== Popup Strategy ===================== */
class PopUpStrategy : public INotificationStrategy {
public:
    void sendNotification(string content) override {
        cout << "Popup Notification:\n" << content << endl;
    }
};

/* ===================== Notification Engine ===================== */
class NotificationEngine : public IObserver {
private:
    NotificationObservable* observable;
    vector<INotificationStrategy*> strategies;

public:
    NotificationEngine() {
        observable = NotificationService::getInstance()->getObservable();
        observable->addObserver(this);
    }

    void addNotificationStrategy(INotificationStrategy* strategy) {
        strategies.push_back(strategy);
    }

    void update() override {
        string content = observable->getNotificationContent();
        for (auto s : strategies) {
            s->sendNotification(content);
        }
    }

    ~NotificationEngine() {
        for (auto s : strategies)
            delete s;
    }
};

/* ===================== MAIN ===================== */
int main() {
    NotificationService* service = NotificationService::getInstance();

    Logger* logger = new Logger(service->getObservable());

    NotificationEngine* engine = new NotificationEngine();
    engine->addNotificationStrategy(new EmailStrategy("nehasaniya@gmail.com"));
    engine->addNotificationStrategy(new SMSStrategy("192783816832"));
    engine->addNotificationStrategy(new PopUpStrategy());

    INotification* notification = new SimpleNotification("Your order has been shipped");
    notification = new TimeStampDecorator(notification);
    notification = new SignatureDecorator(notification, "Amazon");

    service->sendNotification(notification);

    delete logger;
    delete engine;

    return 0;
}
