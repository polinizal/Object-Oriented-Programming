# Увод в Обектно-оринетираното програмиране

<slidebreak/>

## Същност

***Парадигма***, включваща концепция и принципи за имплементиране на програма, при която в основата на моделирането стоят обектите, а не поредицата от инструкции

<slidebreak/>

### **Принципи на ООП:**

- **Абстракция** - Скриване на имплементацията и ненужните детайли от потребителя на обекта, който се интересува само от резултата

- **Капсулация** - обединяване и изолация на самодостатъчните вътрешнти характеристики на обектите

- **Наследяване** - създаване на себеподобни наследници, надграждащи съществуващите характеристики на обектите

- **Полиморфизъм** - използване на наследници, без да се интересуваме от техния специфичен контекст

<slidebreak/>

##  Структури

- Групиране на елементи
- Елементите се наричат членове (член-променливи, член-данни)
- Елементите могат да бъдат от произволни типове

```cpp
struct Point 
{
    double x;
    double y;
    double z;
};
 ```

<slidebreak/>

##  Деклариране на обекти

- оператор `.` - direct member selection

 ```cpp
Point p1; // default values for x,y,z

Point p2 = {3, 4.2}; // 3, 4.2, 0

Point p3;
p3.x = 3;
p3.y = 4.2;
p3.z = true;
 ```
	
<slidebreak/>

## Подаване във функция

- По копие -> избягваме (скъпи са при структурите)
- Ако няма да променяме обекта -> константна референция
- Ако ще променяме обекта -> референция

```cpp
double distanceToCenter(Point p)
{
    return sqrt(p.x * p.x + p.y * p.y);
}

void printPoint(const Point& p) 
{
    std::cout << p.x << "," << p.y; 
}

void mirrorPoint(Point& p)
{
    p.x = - p.x;
    p.y = - p.y;
}
```

<slidebreak/>

## Работа с динамични обекти

- Оператор `->` - indirect member selection

 ```cpp
struct Point 
{
	double x,y;
} p1;

void mirrorPoint(Point* p)
{
    p->x = - p->x;
    p->y = - p->y;
}

void printPoint(const Point& p) 
{
    std::cout << p.x << "," << p.y; 
}

int main() 
{
    p1.x = 42;

	Point* pointPtr = new Point();

	(*pointPtr).x = 3;
	pointPtr->y = 4.2;
	
	printPoint(pointPtr);
	
	delete pointPtr; 
}
```

<slidebreak/>

## Влагане на обекти

```cpp
struct Point 
{
    double x,y,z;
};

struct Line 
{
    char identifier[1024+1];
    Point p1, p2;
};

int main() 
{
    Line l = { "my-line", { 1, 2 }, { 4, 5, 6 } };
    std::cout << l.identifier;
}
```

<slidebreak/>

## Масиви от обекти
 ```cpp
struct Point 
{
	double x,y,z;
};

int main() {
	Point arr1[30]; 
	Point* arr2 = new Point[20];
	
	std::cout << arr1[0].x << " " << arr2[3].y;
	delete[] arr2;
}
```

<slidebreak/>

## Представяне в паметта и подравняване

- В паметта променливите са разположени в същия ред, в който са декларирани в структурата
- Размерът (size) на една структура е мястото в паметта, което заема - оператор `sizeof`
- Подравняването (alignment) на една структура е кратността на адреса, на който може да се запише структурата - оператор `alignof`

<slidebreak/>

- Големината на структурата е винаги кратна на размера на най-големия примитивен тип данни в нея
- Всяка член-променлива се поставя на адрес, делящ се на подравняването ѝ
- Първата променлива от структурата я поставяме на адрес 0
- Подравняването на структурата е равно на най-голямото подравняване на примитивен тип, който съдържа

> [!NOTE]
> Примитивните типове може да ги мислим като структури, при които подравняването и размера са еднакви и определени от самите типове

> [!NOTE]
> Празните незаети байтове се наричат padding

<slidebreak/>

- Ако имаме за член-данна масив с n елемента, то може да си ги мислим като n член-данни от съответния тип
- Размерът на празна структура (без член-данни) е равен на 1
- За вложени структури взимаме предвид подравняването на вложената структура, за да знаем на какъв адрес може да я сложим, както и размерът на член-данните ѝ за определяне на подравняването на цялата структура 

> [!IMPORTANT]
> За да бъде минимална паметта, която ползва една структура, трябва да подредим нейните член-данни в нарастващ или намаляващ ред по големина на променливата

<slidebreak/>

```cpp
struct Example1 
{
    bool b;
    int c;
} ex1;

struct Example2 {
    short int s;  
    char c;  
    int i; 
} ex2;

struct Example3 
{
    char c;         
    double d;    
    int s;          
} ex3;

struct Example4
{
    char c;
    Example1 f1;
    int i;
    Example2 f2;
    double d;
    Example3 f3;
} ex4;

std::cout << alignof(ex1) << " " << sizeof(ex1) << std::endl;
std::cout << alignof(ex2) << " " << sizeof(ex2) << std::endl;
std::cout << alignof(ex3) << " " << sizeof(ex3) << std::endl;
std::cout << alignof(ex4) << " " << sizeof(ex4) << std::endl;
```

<slidebreak/>

## Little-endian + Big-endian

Съществуват следните два начина за запазване на байтовете на типове данни, които имат размер по-голям от 1 байт:
- **Little-endian** - Най-старшият байт е на последна позиция, най-младшият е на първа
- **Big-endian** - Най-старшият байт е на първа позиция, най-младшият е на последна

<slidebreak/>

## Обединения

- Вид структура, при която член-данните споделят една и съща памет
- Едно обединение заема памет, колкото е големината на най-голямата му променлива в обединението (max sizeof)
- Във всеки един момент обаче може да се ползва ефективно само една променлива от обединението

```cpp
union Example 
{
    int a;
    char b;
} un;

int main()
{
    un.a = 65;
    std::cout << var.a << " " << var.b; // 65 A
}
```

<slidebreak/>

```cpp
union Example
{
    int a;
    char b;
    double c;
    float d;
};

int main()
{
    std::cout << sizeof(Example) << " " << alignof(Example); // 8 8
}
```


<slidebreak/>

## Енумерации
- Тип с ограничен домейн от стойности
- Стойностите са от целочислен тип - основен тип на енумерацията
- Eнумерацията има същия размер, представяне на стойност и изисквания за подравняване като основния си тип

```cpp
enum TestEnum 
{
	Option1, //0
	Option2, //1
	Option3, //2
};

enum TestEnum : char 
{
	Option1, //0
	Option2, //1
	Option3, //2
};

int main() 
{
	std::cout << sizeof(IceCream1) << std::endl; // 4
	std::cout << sizeof(IceCream2) << std::endl; // 1
}
```

<slidebreak/>

## Видове енумерации

- Обиктовените енумерации (enum) са unscoped - техните стойности живеят в scope-a на декларация на енумерацията и се преобразуват имплицитно до други енумерации и числени стойности
- Класовите енумерации (enum class) са scoped - техните стойности живеят само в scope-a на енумерацията и не се преобразуват имплицитно до други енумерации и числени стойности

```cpp
enum Color { red, green, blue };                     
enum Card { red_card, green_card, yellow_card }; 
enum class Animal { dog, deer, cat, bird, human }; 
enum class Mammal { kangaroo, deer, human };

int main()
{
    // Bad
    Color color = Color::red;
    Card card = Card::green_card;
    int num = color;    // no problem (bad)
    if (color == Card::red_card) // no problem (bad)
        cout << "bad" << endl;
    if (card == Color::green) // no problem (bad)
        cout << "bad" << endl;

    // Good
    Animal a = Animal::deer;
    Mammal m = Mammal::deer;
    int num2 = a; // error
    if (m == a) // error (good)
        cout << "bad" << endl;
    if (a == Mammal::deer) // error (good)
        cout << "bad" << endl;
}
```

> [!IMPORTANT]
> Винаги ползваме enum class

<slidebreak/>

## Задачи

**Задача 1:** Въвежда се цяло число N и последователно толкова на брой триъгълници (идентификатор + координати на върховете). Отпечатайте идентификаторите на тръгълниците, сортирани по лицата им.

**Задача 2:** Резлизирайте структура Complex, която е за работа с комплексни числа. Имплементирайте функции за събиране, изваждане, умножение и деление. Имплементирайте и функция, която приема инстанция(обект) от новия тип и връща дали е реално число. 