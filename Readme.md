# Компилятор

![alt text](images/comp2.drawio.svg)
## В конце прошлого семестра был написан мой собственный язык программирования. Для исполнения написанных программ код сначала преобразовывался в AST, а затем транслировался в ассемблер для моей эмуляции процессора. К сожалению, исполнение на эмулированном процессоре занимает больше времени, чем на реальном. Итак, основная цель работы - увеличение производительности написанного на моем языке кода. Для этого надо сразу преобразовывать в код, который сможет исполняться на нативной архитектуре - в данном случае x86_64.

## Архитектура виртуального процессора
### Инструкции архитектуры могут быть поделены на следующие группы:
### 1. Инструкции, используемые для арифметических операций: ADD, SUB, MUL, DIV
### 2. Условные и безусловные 'jump'ы: JMP, JNE, JE, ...
### 3. Инструкции перемещения данных - PUSH и POP
### 4. Инструкция вывода OUT
<br/>

## Арифметика
### Все вычисления происходят в стеке, то есть если инструкция требует 2-х аргументов, то они лежат на вершине стека, результат вычисления кладется на вершину стека.
### Пример выполнения ADD:
```C
stack_pop(&proc->stk, &second_argument);
stack_pop(&proc->stk, &first_argument);
stack_push(&proc->stk, (second_argument+first_argument));
```
<br/>

## Перемещение данных
### Для перемещения данных используются две инструкции - PUSH и POP. Инструкция PUSH добавляет значения в стек, а POP - удаляет его из стека. Есть возможность 'push'ить значения регистра и 'pop'ить в регистр, других вариантов не предусмотрено.
<br/>

## Вывод
### Инструкция OUT берет число с вершины стека и, используя printf, распечатывает его в консоль. Аргумент инструкции OUT удаляется из стека. 

<br/>

# Backend X86
![alt text](images/1.drawio.svg)
### Бекенд транслирует промежуточное представление [Cringe Language](https://github.com/andrushechka37/CringeLanguage) в инструкции X86 процессора. После чего nasm создает исполняемый файл.
### Трансляция не происходит напрямую из дерева в X86 инструкции, а сначала происходит формирование IR(Промежуточное представление)
### Элемент IR представляет собой структуру:
```C
struct IR_element {
    int operator_number;        // номер оператора
    types_of_node type;         // тип элемента
    IR_argument  argument;      // аргумент оператора, если его аргумент может быть только простым выражением
    int quantity_of_arg;        // количество аргументов оператора
    int label;                  // номер метки
};
```
### Это сделано для того, чтобы:
### Во-первых, такое промежуточное представление может быть использовано для оптимизаций. Например, в таком представлении хорошо видны последовательные PUSH / POP. Часто в таких случаях можно отказаться от использования стека. На данный момент никакие оптимизации не применяются.
<br/>

### Во-вторых, IR полезен, когда необходимо создавать исполняемый код под разные архитектуры. Так, не придется для каждой конкретной архитектуры писать общие оптимизации заново - все они могут быть произведены на стадии промежуточного представления, а значит, нужно будет написать только перевод из IR в инструкции для новой архитектуры.
<br/>

## Архитектура
### Архитектура соответствует архитектуре X86
<br/>

## Арифметика
### Арифметика в оттранслированном коде функционирует аналогично арифметике виртуального процессора. На момент произведения арифметического действия, на вершине стека ожидаются два аргумента. Оба значения помещаются в зарезервированные под вычисления регистры, производятся математические действия, после чего результат кладется на вершину стека. В качестве стека используется аппаратный стек архитектуры х86.

### Пример трансляции SUB

```assembly
pop r12 
pop r13
sub r13, r12
push r13
```
<br/>

## Перемещение данных
### Перемещение данных также происходит через стек при помощи PUSH и POP, а не через mov в силу скорого начала сессии.
<br/>

## Инструкции сравнения
### Аргументы сравнения должны находится на верхушке стека, откуда они берутся для сравнения.
### Пример трансляции:

```assembly
pop r12
pop r13
cmp r13, r12
jne some_label
```
<br/>

## Вывод
### Инструкция OUT транслируются в вызовы соответствующих функций стандартной библиотеки функций, написанной специально для этих инструкций. Функция для трансляции OUT содержится в `stdlib.s`.
<br/>

# Сравнение времени исполнения
### Для сравнения времени исполнения виртуальным процессором и транслированной в X86 программы используем программу включающую в себя большой объем вычислений. Для проведения теста используем компилятор, написанный в качестве учебной задачи в первом семестре. Результатом работы компилятора является код, содержащий инструкции архитектуры виртуального процессора. Для измерения времени будем использовать функцию `rdtsc`. Считался факториал 10 100000 раз.
### Исполняемая программа:
```
{
    x = 12;
    $factorial;
    print(answer);

    $factorial {

        {
            if (x = 1;)
            answer = 1;
            ret;
        }

        {
            x = x - 1;
            $factorial;

            x = x + 1;
            answer = answer * x;
        }
    };
}
```


|                | CPU | X86 backend |
|:--------------:|:---:|:-----------:|
| Ticks * $10^5$ |  41 |      14     |


### Исполнение ускорилось в ~ 3 раза, что является неплохим результатом, но можно было его сильно улучшить, если бы применялись оптимизации на уровне IR, а передача данных осуществлялась бы не только через стек.

# Известные проблемы:
## При повторном вызове функции print второй вывод неправильный, из-за загаживания какиx-то регистров(вроде)
```
{
   x = 123;
   y = 456;
   print(x);
   print(y);
}
```