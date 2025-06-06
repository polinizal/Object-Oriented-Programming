# Множества

Разглеждаме абстрактен базов клас `Множество`, което може да съдържа елементи от цели **32**-битови числа и задължително притежава операция за проверка дали даден елемент от съответния тип принадлежи на множеството.

Да се реализират следните конкретни наследници на абстрактния базов клас `Множество`:

- **Множество по критерий** – в конструктора се подава предикат (това е булева функция или обект, който се държи като такава), който по подаден ѝ като аргумент елемент решава дали той да принадлежи на множеството или не.
- **Сечение на множества** – в конструктора се подават няколко множества и създаденият обект (сечение) трябва да съдържа точно онези елементи, които се съдържат във всяко от посочените множества. Елементите на всички множества са от един и същ тип.
- **Обединение на множества** – в конструктора се подават няколко множества и създаденият обект (обединение) трябва да съдържа точно онези елементи, които се съдържат в поне едно от посочените множества. Елементите на всички множества са от един и същ тип.

Да се реализира програма, която прочита от двоичeн файл `set.dat` информация за множество и конструира ново множество съгласно указаните в двоичния файл правила.

Двоичният файл има следната структура:
- две цели неотрицателни **16**-битови числа `N` и `Т`, където стойността на `N` не надхвърля **32**
- стойността на `T` определя формата на двоичния файл по-нататък и как се конструира съответното множество, както следва:
	- **0** - следват N цели **32**-битови числа, които определят крайно множество, състоящо се точно от тях
	- **1** - следват N цели **32**-битови числа, които определят крайно множество, състоящо се от  точно тези цели **32**-битови числа, които не се делят на нито едно от дадените числа
	- **2** - следват N цели **32**-битови числа, които определят крайно множество, състоящо се от  точно тези цели **32**-битови числа, които се делят на точно едно от дадените числа
	- **3** - следват N низа, всеки от тях терминиран с 0, които описват пътища към файлове, задаващи множества, чиито обединение представя текущото множество
	- **4** - следват N низа, всеки от тях терминиран с 0, които описват пътища към файлове, задаващи множества, чиито сечение представя текущото множество

**Програмата да работи в два режима:**

1. Въвежда от стандартния вход две цели числа `a` и `b` и извежда всички числа в интервала `[a; b]`, които са в построеното множество.
Позволява последователно генериране на всички елементи от множеството, като всеки следващ елемент се генерира при поискване от потребителя.
2. Да се обработват по подходящ начин различните грешки, свързани с некоректен вход.

**Пример:**

| set.dat 			| first.dat			| second.dat		| third.dat			|
| ----------------- | ----------------- | ----------------- | ----------------- |
| 3 4				| 7 0				| 2 1				| 3 2				|
| first.dat			| 1 3 4 6 2 8 9		| 4 6				| 2 3 5				|
| second.dat		|					|					|					|
| third.dat			|					|					|					|

При въведени числа `a = 0` и `b = 10`, се очаква да се изведат числата 2, 3 и 9 в някакъв ред. 

**Забележка:** Съдържанието на двоичните файлове е показано като текст само за удобство на примера. **Файловете трябва да са двоични!**
