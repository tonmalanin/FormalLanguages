## Автор
Маланьин Антон Олегович
Б05-327
# Сборка и запуск проекта
## Сборка
Через командную строку перейти в директорию проекта и ввести следующие команды:
```
cmake -S . -B build
cmake --build build
```
## Запуск
```
./bin/fl.exe [ОПЦИИ] [строка]
Опции:
    -i, --input regexp - на вход даётся регулярное выражение
                json - на вход даётся конечный автомат заданный json-объектом
    -r, --result nfa - будет построен НКА без ε-переходов
                 dfa - будет построен ПДКА
                 min-cdfa - будет построен МПДКА
```
## Формат входных данных
Регулярное выражение строится по следующим правилам:
- "a", где a - ASCII-символ, является регулярным выражением
- Если "s", "t" - регулярные выражения, то регулярным выражением является:
    - "(s)"
    - "st"
    - "s|t"
    - "s*"
    - "s+"

Вместо символа 'ε' в выражении следует использовать символ '#'

json-объект должен содержать следующие поля:
- s0 - начальное состояние
- states - список состояний
- final - список конечных состояний
- delta - список переходов. Переходы представлены json-объектами с полями:
    - from
    - to
    - sym

Состояния задаются строками
## Требования к среде
Должны быть установлены CMake, версия не меньше 3.13, и компилятор C++. Для запуска тестов необходимо установить GTest
# Описание проекта
## Устройство проекта
Класс Automaton используется для хранения конечных автоматов. Содержит поля: start - список начальных состояний; state_num - количество состояний; is_final - список, хранящий информацию про конечность состояний; delta - список переходов; alphabet -  алфавит языка, в котором действует автомат. Для хранения переходов использкется структура Transition. В файле Automaton.cpp реализованы преобразования между json-объектами и классом Automaton, а также операции объединения, конкатенации и замыкания Клини языков, задаваемых автоматами. В файле AutomatonFromRegularExpression.cpp реализовано построение автомата по регулярному выражению. В файле MakeDeterministic.cpp реализованы алгоритмы построения автомата без ε-переходов и ПДКА. В файле MakeMinimal.cpp реализован алгоритм построения МПДКА.
## Функционал
По данному регулярному выражению или json-объекту строит НКА без ε-переходов, ПДКА (относительно алфавита, состоящего из неслужебных символов исходного регулярного выражения/символов переходов исходного json-объекта) или МПДКА и выводит в формате json.
