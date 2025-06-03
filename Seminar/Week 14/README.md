# Design patterns & SOLID principles

<slidebreak/>

## Design Patterns

- Шаблони, които решават общоизвестни проблеми в дизайна на кода.
- Помагат за създаване на по-структуриран, четим и поддържан код.
- Трябва да се ползват правилно и по предназначение.
- [Refactoring Gure](https://refactoring.guru/design-patterns)

<slidebreak/>

## Creational Patterns

- Отнасят се до начина на създаване на обекти.

### Singleton

- Гарантира, че даден клас има само един екземпляр и предоставя глобална точка за достъп до него.
- На пръв поглед това изглежда да може да се постигне просто със статични променливи и методи, но реално инстанцията на класа носи своите предимства, от това, че е клас, а именно
    - Може да наследява други класове и да се спазва добра абстракция и dependency injection.
    - Може да бъде мокван при тестове.
- Като минус може да се отнесе факта, че не е thread-safe, но това е за други курсове.

```cpp
class ConfigurationManager {
private:
    std::string configData;

    ConfigurationManager() {
        configData = "Default Config";
    }

    ConfigurationManager(const ConfigurationManager& other) = delete;
	ConfigurationManager& operator=(const ConfigurationManager& other) = delete;

public:

    static ConfigurationManager* getInstance() {
        static ConfigurationManager* instance = new ConfigurationManager();
        return instance;
    }

    std::string getConfigData() {
        return configData;
    }

    void setConfigData(const std::string& data) {
        configData = data;
    }
}
```

<slidebreak/>

### Factory

- Използва се за създаване на обекти без да се специфицира конкретният клас на обекта, който ще бъде създаден.
- Позволява на клиентския код да работи с абстрактни класове или интерфейси, вместо с конкретни реализации относно начина на създаване на обектите.

```cpp
#include <iostream>

class Base {
public:
	virtual ~Base() = default;
};
class Der1 : public Base {};
class Der2 : public Base {};

// Factory Method
class BaseFactory {
public:
	virtual Base* create() const = 0;
	virtual ~BaseFactory() = default;
};

class Der1Factory : public BaseFactory {
public:
	Base* create() const override {
		return new Der1();
	}
};

class Der2Factory : public BaseFactory {
public:
	Base* create() const override {
		return new Der2();
	}
};

enum class ChildType {
    Der1,
    Der2
};

// Factory
BaseFactory* factoryOfFactories(ChildType type) {
    switch (type) {
        case ChildType::Der1:
            return new Der1Factory();
        case ChildType::Der2:
            return new Der2Factory();
        default:
            throw std::invalid_argument("Unknown type");
    }
}


void runApp(BaseFactory* factory) {
    Base* obj = factory->create();
    //...
}

int main() {
	BaseFactory* factory = factoryOfFactories(true);
	runApp(factory);
	delete factory;
}
```

<slidebreak/>


## Structural Patterns

- Отнасят се до начина на организация на класове и обекти.

### Adapter

- Позволява на обекти с несъвместими интерфейси да работят заедно.
- A иска да работи с B. Ние имаме C, което не е B. Правим адаптер D, който е B и използва C.

```cpp
class MyStringWriter : public std::ostream {
    std::stringbuf buffer;
public:
    MyStringWriter() : std::ostream(&buffer) {}

    MyString str() const {
        return buffer.str();
    }
};

class Person {
public:
    MyString name;
    int age;

    Person(const MyString& name, int age) : name(name), age(age) {}

    void print(std::ostream& os) const {
        os << "Name: " << name << ", Age: " << age;
    }
}

int main() {
    MyStringWriter writer;
    Person person("John", 30);
    person.print(writer);
    
    // Use the MyString object with the data later 
    std::cout << "Output: " << writer.str() << std::endl;
    
    return 0;
}
```

<slidebreak/>

### Composite

- Позволява да се създадат дървовидни структури от обекти, където всеки обект може да бъде както лист, така и композитен обект.
- Трябва общ интерфейс за всички обекти в дървото, който да дефинира общи операции.

```cpp
class FileSystemEntity {
public:
	virtual size_t getSize() const = 0;
	virtual ~FileSystemEntity() = default;
};

class File : public FileSystemEntity {
	size_t size = 0;
public:
	size_t getSize() const {
		return size;
	}
};

class Directory : public FileSystemEntity {
	Vector<shared_ptr<FileSystemEntity>> children;
public:
	size_t getSize() const {
		size_t size = 0;
		for (size_t i = 0; i < children.getSize(); i++) {
			size += children[i]->getSize();
		}

		return size;
	}
};
```

<slidebreak/>

## Behavioral Patterns

- Отнасят се до начина, по който обектите взаимодействат помежду си.

### Observer

- Позволява на един обект (subject) да уведомява други обекти (observers) за промени в своето състояние.

```cpp

class Notification {
    std::string message;
public:
    Notification(const std::string& msg) : message(msg) {}

    std::string getMessage() const {
        return message;
    }
}

class EventNotification : public Notification {
    size_t eventId;
public:
    EventNotification(size_t id, const std::string& msg) 
        : Notification(msg), eventId(id) {}

    size_t getEventId() const {
        return eventId;
    }
}

class NotificationObserver {
public:
    virtual void update(const Notification& notification) = 0;
    virtual ~Observer() = default;
};

class EmailAlertHandler : public NotificationObserver {
public:
    void update(const Notification& notification) override {
        // Send email alert with notification message
        std::cout << "Email Alert: " << notification.getMessage() << std::endl;
    }
};


class EventManager {
    Vector<NotificationObserver*> observers;

public:

    void addObserver(NotificationObserver* observer) {
        observers.push_back(observer);
    }

    void removeObserver(NotificationObserver* observer) {
        observers.remove(observer);
    }

    void notifyObservers(const Notification& notification) {
        for (auto& observer : observers) {
            observer->update(notification);
        }
    }

    void startEvent(size_t eventId){
        // ...
        EventNotification notification(eventId,"Event started!");
        notifyObservers(notification); // Notify all observers
    }
};
```

<slidebreak/>

### Visitor 

- Позволява взаимно разпозване на обектите

```cpp

class Circle;
class Rectangle;

class Shape {
public:
    virtual bool intersects(const Shape* other) const = 0;
    
    virtual bool intersects(const Circle& circle) const = 0;
    virtual bool intersects(const Rectangle& rectangle) const = 0;

    virtual ~Shape() = default;
};

class Circle : public Shape {
public:
    bool intersects(const Shape* other) const override {
        return other->intersects(*this);
    }

    bool intersects(const Circle& circle) const override {
        // Logic for circle-circle intersection
        return true;
    }

    bool intersects(const Rectangle& rectangle) const override {
        // Logic for circle-rectangle intersection
        return true; 
    }
};

class Rectangle : public Shape {
public:
    bool intersects(const Shape* other) const override {
        return other->intersects(*this);
    }

    bool intersects(const Circle& circle) const override {
        // Logic for rectangle-circle intersection
        return true;
    }

    bool intersects(const Rectangle& rectangle) const override {
        // Logic for rectangle-rectangle intersection
        return true; 
    }
};
```

<slidebreak/>

### Iterator 

- Позволява последователен достъп до елементите на колекция без да се разкрива вътрешната й структура (например може да е дървовидна).

```cpp

template<typename T>
class Iterator {
    virtual bool hasNext() const = 0;
    virtual T next() = 0;
};

class MyCollection {
    Vector<int> data;
public:
    void add(int value) {
        data.push_back(value);
    }

    class MyCollectionIterator : public Iterator<int> {
        MyCollection& collection;
        int index;

    public:
        Iterator(MyCollection& collection) : collection(collection), index(collection.size() - 1) {}

        bool hasNext() const {
            return index >= 0;
        }

        int next(){
            return collection.data[index--];
        }
    };

    Iterator iterator() {
        return MyCollectionIterator(*this, 0);
    }
};
```

<slidebreak/>

## Command

- Позволява обработка на команди като обекти. Това от своя страна позволява лесно изпълнение, отмяна и повторно изпълнение на команди - съответно с различни параметри.

```cpp
class Command {
public:
    virtual ~Command() = default;
    virtual void execute(const std::vector<std::string>& args) = 0;
};

class PrintHelloCommand : public Command {
public:
    void execute(const std::vector<std::string>& args) override {
        std::cout << "Hello, world!" << std::endl;
    }
};

class PrintDateCommand : public Command {
public:
    void execute(const std::vector<std::string>& args) override {
        std::time_t now = std::time(nullptr);
        std::cout << "Current date and time: " << std::ctime(&now);
    }
};

class CommandManager {
public:
    void registerCommand(const std::string& name, std::unique_ptr<Command> cmd) {
        commands[name] = std::move(cmd);
    }

    void executeCommand(const std::string& line) {
        std::istringstream iss(line);
        std::string cmdName;
        iss >> cmdName;

        std::vector<std::string> args;
        std::string arg;
        while (iss >> arg) {
            args.push_back(arg);
        }

        auto it = commands.find(cmdName);
        if (it != commands.end()) {
            it->second->execute(args);
        } else {
            std::cout << "Unknown command: " << cmdName << std::endl;
        }
    }

private:
    std::map<std::string, std::unique_ptr<Command>> commands;
};

int main() {
    CommandManager manager;

    manager.registerCommand("hello", std::make_unique<PrintHelloCommand>());
    manager.registerCommand("date", std::make_unique<PrintDateCommand>());

    std::string input;
    std::cout << "Enter command (hello, date, exit): ";

    while (std::getline(std::cin, input)) {
        if (input == "exit") break;
        invoker.executeCommand(input);
        std::cout << "\nEnter command: ";
    }

    return 0;
}
```

<slidebreak/>

## SOLID Principles

- Принципи за обектно-ориентирано програмиране, които помагат за създаване на по-гъвкав, четим и поддържан код.

<slidebreak/>

### Single Responsibility Principle (SRP)

- Всеки клас трябва да има само една отговорност и да бъде отговорен съмо за една част от функционалността на приложението.

<slidebreak/>

### Open/Closed Principle (OCP)

- Класовете трябва да бъдат отворени за разширение, но затворени за промяна.
- Това означава, че трябва да можем да добавяме нова функционалност чрез наследяване или композиция, без да променяме съществуващия код.

<slidebreak/>

### Liskov Substitution Principle (LSP)

- Обектите от подкласове трябва да могат да бъдат използвани вместо обекти от суперкласове без да нарушават функционалността на програмата.

<slidebreak/>

### Interface Segregation Principle (ISP)

- Клиентите не трябва да бъдат принуждавани да зависят от интерфейси, които не използват.
- Вместо това, трябва да се създават по-малки и специфични интерфейси, които да отговарят на нуждите на клиентите.

<slidebreak/>

### Dependency Inversion Principle (DIP)

- Високо ниво модули не трябва да зависят от ниско ниво модули. И двете трябва да зависят от абстракции.
- Абстракциите не трябва да зависят от детайли. Детайлите трябва да зависят от абстракции.

<slidebreak/>

## Пример

```cpp
class IShape {
public:
    virtual double area() const = 0;
    virtual ~IShape() = default;
};

class IDrawable {
public:
    virtual void draw() const = 0;
    virtual ~IDrawable() = default;
}

class Circle : public IShape, public IDrawable {
    double radius;
public:
    Circle(double r) : radius(r) {}

    double area() const override {
        return PI * radius * radius;
    }

    void draw() const override {
        std::cout << "Drawing Circle with radius: " << radius << std::endl;
    }
};

class Rectangle : public IShape {
    double width, height;
public:
    Rectangle(double w, double h) : width(w), height(h) {}

    double area() const override {
        return width * height;
    }
};

class Square : public IDrawable {
    double side;
public:
    Square(double s) : side(s) {}

    void draw() const override {
        std::cout << "Drawing Square with side: " << side << std::endl;
    }
};

class AreaCalculator {
public:
    double calculateArea(const IShape& shape) const {
        return shape.area();
    }
};

class ShapeDrawer {
public:
    void drawShape(const IDrawable& drawable) const {
        drawable.draw();
    }
};
```