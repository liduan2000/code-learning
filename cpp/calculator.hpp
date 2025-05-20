#pragma once

#include <stack>
#include <string>
#include <unordered_map>
#include <vector>

/*
1. 遇到操作数（数字/变量）：直接加入输出队列
2. 遇到左括号 (：入栈
3. 遇到右括号 )：不断弹出栈顶运算符直到遇到左括号
4. 遇到运算符 + - * /：
    如果栈顶是比当前运算符[优先级高或相等]的运算符，就弹出
    然后把当前运算符入栈
5. 最后：把栈里剩下的运算符全部弹出加入输出

需要特别注意对于负号的处理：前面加0
*/

class Calculator {
  public:
    int calculate(const std::string& s) {
        std::vector<std::string> tokens = tokenize(s);
        std::vector<std::string> suffix = infixToSuffix(tokens);
        return calculateSuffix(suffix);
    }

  private:
    const std::unordered_map<std::string, int> priorities{{"+", 1}, {"-", 1}, {"*", 2}, {"/", 2}};

    std::vector<std::string> tokenize(const std::string& expr) {
        std::vector<std::string> tokens;
        int i = 0;
        while (i < expr.size()) {
            if (isspace(expr[i])) {
                ++i;
            } else if (isdigit(expr[i])) {
                int j = i;
                while (j < expr.size() && isdigit(expr[j])) { ++j; }
                tokens.push_back(expr.substr(i, j - i));
                i = j;
            } else if /* 负号处理：前面加0 */ (expr[i] == '-' && (i == 0 || tokens.empty() || tokens.back() == "(")) {
                tokens.push_back("0");
                tokens.push_back("-");
                ++i;
            } else {
                tokens.push_back(std::string(1, expr[i]));
                ++i;
            }
        }
        return tokens;
    }

    std::vector<std::string> infixToSuffix(const std::vector<std::string>& infix) {
        std::vector<std::string> suffix;
        std::stack<std::string> operators;
        for (const std::string& s : infix) {
            if (s == "(") {
                operators.push(s);
            } else if (s == ")") {
                while (!operators.empty() && operators.top() != "(") {
                    suffix.push_back(operators.top());
                    operators.pop();
                }
                if (!operators.empty()) { operators.pop(); }
            } else if (priorities.contains(s)) {
                while (!operators.empty() && operators.top() != "(" &&
                       priorities.at(operators.top()) >= priorities.at(s)) {
                    suffix.push_back(operators.top());
                    operators.pop();
                }
                operators.push(s);
            } else {
                suffix.push_back(s);
            }
        }
        while (!operators.empty()) {
            suffix.push_back(operators.top());
            operators.pop();
        }
        return suffix;
    }

    int calculateSuffix(const std::vector<std::string>& suffix) {
        std::stack<int> st;
        for (const std::string& s : suffix) {
            if (priorities.contains(s)) {
                int b = st.top();
                st.pop();
                int a = st.top();
                st.pop();
                if (s == "+") {
                    st.push(a + b);
                } else if (s == "-") {
                    st.push(a - b);
                } else if (s == "*") {
                    st.push(a * b);
                } else {
                    st.push(a / b);
                }
            } else {
                st.push(std::stoi(s));
            }
        }
        return st.top();
    }
};
