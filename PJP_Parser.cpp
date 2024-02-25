#include <iostream>
#include <string>
#include <stack>
#include <cctype>
#define STR(a) (a-'0')
using namespace std;


bool tryParse(const string& str, int& result) {// return if string is valid integer
    try {
        size_t pos;
        result = stoi(str, &pos);
        return pos == str.length();
    }
    catch (const invalid_argument& e) {
        return false;
    }
    catch (const out_of_range& e) {
        return false;
    }
}
void Multiply(stack<int>* st, char* op) {
    int a, b;
    b = st->top();
    st->pop();
    a = st->top();
    st->pop();
    if (*op == '*')
        st->push(a * b);
    else if (*op == '/')
        st->push(a / b);
    *op = ' ';
}
bool calculate(string sub, int* result) {
    stack<int> operandStack;
    bool isDigit = false;
    int isNegative = 1;
    char multiply = ' ';

    for (int i = 0; i < sub.length(); i++) {
        if (isdigit(sub[i])) {
            if (isDigit) {
                int num = operandStack.top() * 10 + (STR(sub[i]) * isNegative);
                operandStack.pop();
                operandStack.push(num);
            }
            else {// first digit
                isDigit = true;
                operandStack.push(STR(sub[i]) * isNegative);
            }
        }
        else if (isDigit) {// operator // isDigit  digit must be in front of each operator
            if (multiply != ' ') // operand before was product or divide
                Multiply(&operandStack, &multiply);

            isDigit = false;// flag
            isNegative = 1;//flag
            if(sub[i] == '-')
                isNegative = -1;
            else if (sub[i] == '*' || sub[i] == '/')
                multiply = sub[i];
        }
        else // ** ++
            return false;
    }
    if (multiply != ' ')
        Multiply(&operandStack, &multiply);

    *result = 0;
    while(!operandStack.empty())
    {
        *result += operandStack.top();
        operandStack.pop();
    }
    return true;
}
void expression(string expr) {
    expr.erase(remove_if(expr.begin(), expr.end(), ::isspace), expr.end());

    int result, start, end;
    stack<int> bracketStack;

    while (!tryParse(expr, result)) {
        for (int i = 0; i < expr.length(); i++) {
            char ch = expr[i];
            if (ch == '(') {
                if (i != 0 && (isdigit(expr[i - 1]) || expr[i - 1] == ')')) {// operator in front of left bracket if is not at the start of expression
                    cout << "ERROR 0" << endl;
                    return;
                }
                bracketStack.push(i);
            }
            else if (ch == ')') {
                if (bracketStack.empty() || !isdigit(expr[i-1])) { // no left bracket first
                    cout << "ERROR 1" << endl;
                    return;
                }
                start = bracketStack.top();
                bracketStack.pop();
                end = i;
                if (!calculate(expr.substr(start + 1, end - start - 1),&result)) {// invalid expression inside of bracket
                    cout << "ERROR 2" << endl;
                    return;
                }
                expr.replace(start, end - start + 1, to_string(result));
                i -= end - start + 1;
                cout << expr << endl;
            }
            else if (i == expr.length() - 1 && bracketStack.empty()) {// invalid expression
                if (!calculate(expr,&result)) {
                    cout << "ERROR 3" << endl;
                    return;
                }
                expr.replace(0, expr.length(), to_string(result));
                cout << expr << endl;
            }
        }

        while (!bracketStack.empty()) {// clear the stack of bracket
            bracketStack.pop();
        }
    }
}
int main() {
    int expCount;
    string text;

    cout << "Enter number of expression: " << endl;
    cin >> expCount;
    for (int j = 0; j < expCount; j++)
    {
        cout << "Enter expression: " << endl;
        cin >> text;
        expression(text);
    }
}
