#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <sstream>
#include <memory>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <random>

using namespace std;

// Simple S-expression parser and transformer
struct SExpr {
    string value;
    vector<shared_ptr<SExpr>> children;
    bool isAtom;

    SExpr(const string& v) : value(v), isAtom(true) {}
    SExpr() : isAtom(false) {}

    shared_ptr<SExpr> clone() const {
        auto result = make_shared<SExpr>();
        result->value = value;
        result->isAtom = isAtom;
        for (const auto& child : children) {
            result->children.push_back(child->clone());
        }
        return result;
    }

    void print(ostream& os) const {
        if (isAtom) {
            os << value;
        } else {
            os << "(";
            for (size_t i = 0; i < children.size(); i++) {
                if (i > 0) os << " ";
                children[i]->print(os);
            }
            os << ")";
        }
    }
};

class Parser {
private:
    string input;
    size_t pos;

    void skipWhitespace() {
        while (pos < input.size() && isspace(input[pos])) {
            pos++;
        }
    }

    shared_ptr<SExpr> parseAtom() {
        skipWhitespace();
        if (pos >= input.size()) return nullptr;

        size_t start = pos;
        if (input[pos] == '-' || isdigit(input[pos])) {
            if (input[pos] == '-') pos++;
            while (pos < input.size() && isdigit(input[pos])) pos++;
        } else {
            while (pos < input.size() && !isspace(input[pos]) && input[pos] != '(' && input[pos] != ')') {
                pos++;
            }
        }

        string value = input.substr(start, pos - start);
        return make_shared<SExpr>(value);
    }

public:
    Parser(const string& s) : input(s), pos(0) {}

    shared_ptr<SExpr> parse() {
        skipWhitespace();
        if (pos >= input.size()) return nullptr;

        if (input[pos] == '(') {
            pos++;
            auto expr = make_shared<SExpr>();
            expr->isAtom = false;

            while (true) {
                skipWhitespace();
                if (pos >= input.size()) break;
                if (input[pos] == ')') {
                    pos++;
                    break;
                }
                auto child = parse();
                if (child) {
                    expr->children.push_back(child);
                } else {
                    break;
                }
            }
            return expr;
        } else {
            return parseAtom();
        }
    }

    vector<shared_ptr<SExpr>> parseAll() {
        vector<shared_ptr<SExpr>> result;
        while (pos < input.size()) {
            skipWhitespace();
            if (pos >= input.size()) break;
            auto expr = parse();
            if (expr) {
                result.push_back(expr);
            } else {
                break;
            }
        }
        return result;
    }
};

string readProgram(istream& is) {
    string result;
    string line;
    while (getline(is, line)) {
        if (line == "endprogram") break;
        result += line + "\n";
    }
    return result;
}

// Code transformer
class Transformer {
private:
    map<string, string> varRenaming;
    map<string, string> funcRenaming;
    int counter;
    mt19937 rng;

    string getNewName(const string& prefix) {
        counter++;
        return prefix + to_string(counter);
    }

    bool isBuiltin(const string& name) {
        static set<string> builtins = {
            "function", "block", "set", "print", "if", "while",
            "array.create", "array.get", "array.set", "array.length",
            "+", "-", "*", "/", "%", "<", ">", "<=", ">=", "==", "!=",
            "and", "or", "not"
        };
        return builtins.count(name) > 0;
    }

    shared_ptr<SExpr> transform(shared_ptr<SExpr> expr, bool inFunction = false) {
        if (!expr) return nullptr;

        if (expr->isAtom) {
            // Rename variables and functions
            string val = expr->value;
            if (!isBuiltin(val) && !val.empty() && !isdigit(val[0]) && val[0] != '-') {
                if (varRenaming.count(val)) {
                    return make_shared<SExpr>(varRenaming[val]);
                }
            }
            return expr->clone();
        }

        auto result = make_shared<SExpr>();
        result->isAtom = false;

        if (expr->children.empty()) {
            return result;
        }

        // Handle function definitions
        if (expr->children[0]->isAtom && expr->children[0]->value == "function") {
            if (expr->children.size() >= 2) {
                result->children.push_back(expr->children[0]->clone());

                // Function name or parameter list
                auto funcName = expr->children[1];
                if (funcName->isAtom) {
                    string newName = getNewName("f");
                    funcRenaming[funcName->value] = newName;
                    varRenaming[funcName->value] = newName;
                    result->children.push_back(make_shared<SExpr>(newName));
                } else if (!funcName->isAtom && !funcName->children.empty()) {
                    // Parameter list (funcname param1 param2...)
                    auto paramList = make_shared<SExpr>();
                    paramList->isAtom = false;

                    map<string, string> oldRenaming = varRenaming;

                    for (size_t i = 0; i < funcName->children.size(); i++) {
                        if (funcName->children[i]->isAtom) {
                            string paramName = funcName->children[i]->value;
                            if (i == 0) {
                                // Function name
                                string newName = getNewName("f");
                                funcRenaming[paramName] = newName;
                                varRenaming[paramName] = newName;
                                paramList->children.push_back(make_shared<SExpr>(newName));
                            } else {
                                // Parameter name
                                string newName = getNewName("p");
                                varRenaming[paramName] = newName;
                                paramList->children.push_back(make_shared<SExpr>(newName));
                            }
                        } else {
                            paramList->children.push_back(funcName->children[i]->clone());
                        }
                    }
                    result->children.push_back(paramList);

                    // Process function body
                    for (size_t i = 2; i < expr->children.size(); i++) {
                        result->children.push_back(transform(expr->children[i], true));
                    }

                    varRenaming = oldRenaming;
                    return result;
                }

                // Process body
                for (size_t i = 2; i < expr->children.size(); i++) {
                    result->children.push_back(transform(expr->children[i], true));
                }
                return result;
            }
        }

        // Handle set statements
        if (expr->children[0]->isAtom && expr->children[0]->value == "set") {
            if (expr->children.size() >= 3) {
                result->children.push_back(expr->children[0]->clone());

                auto varName = expr->children[1];
                if (varName->isAtom && !isBuiltin(varName->value)) {
                    if (!varRenaming.count(varName->value)) {
                        varRenaming[varName->value] = getNewName("v");
                    }
                    result->children.push_back(make_shared<SExpr>(varRenaming[varName->value]));
                } else {
                    result->children.push_back(transform(varName, inFunction));
                }

                for (size_t i = 2; i < expr->children.size(); i++) {
                    result->children.push_back(transform(expr->children[i], inFunction));
                }
                return result;
            }
        }

        // Default: transform all children
        for (const auto& child : expr->children) {
            result->children.push_back(transform(child, inFunction));
        }

        return result;
    }

public:
    Transformer() : counter(0), rng(42) {}

    vector<shared_ptr<SExpr>> transformProgram(const vector<shared_ptr<SExpr>>& program) {
        vector<shared_ptr<SExpr>> result;
        varRenaming.clear();
        funcRenaming.clear();
        counter = 0;

        for (const auto& expr : program) {
            result.push_back(transform(expr));
        }

        return result;
    }
};

// Similarity checker
class SimilarityChecker {
private:
    double computeTreeSimilarity(shared_ptr<SExpr> e1, shared_ptr<SExpr> e2) {
        if (!e1 || !e2) return 0.0;

        if (e1->isAtom && e2->isAtom) {
            return e1->value == e2->value ? 1.0 : 0.0;
        }

        if (e1->isAtom != e2->isAtom) return 0.0;

        if (e1->children.size() != e2->children.size()) {
            return 0.0;
        }

        double sum = 0.0;
        for (size_t i = 0; i < e1->children.size(); i++) {
            sum += computeTreeSimilarity(e1->children[i], e2->children[i]);
        }

        return sum / max(1.0, (double)e1->children.size());
    }

    void extractFeatures(shared_ptr<SExpr> expr, map<string, int>& features) {
        if (!expr) return;

        if (expr->isAtom) {
            features[expr->value]++;
        } else {
            if (!expr->children.empty() && expr->children[0]->isAtom) {
                features["op:" + expr->children[0]->value]++;
            }
            features["list:" + to_string(expr->children.size())]++;

            for (const auto& child : expr->children) {
                extractFeatures(child, features);
            }
        }
    }

public:
    double computeSimilarity(const vector<shared_ptr<SExpr>>& prog1,
                            const vector<shared_ptr<SExpr>>& prog2) {
        if (prog1.empty() || prog2.empty()) return 0.5;

        // Structure similarity
        double structSim = 0.0;
        size_t minSize = min(prog1.size(), prog2.size());
        size_t maxSize = max(prog1.size(), prog2.size());

        for (size_t i = 0; i < minSize; i++) {
            structSim += computeTreeSimilarity(prog1[i], prog2[i]);
        }
        structSim /= maxSize;

        // Feature-based similarity
        map<string, int> features1, features2;
        for (const auto& expr : prog1) {
            extractFeatures(expr, features1);
        }
        for (const auto& expr : prog2) {
            extractFeatures(expr, features2);
        }

        set<string> allFeatures;
        for (const auto& p : features1) allFeatures.insert(p.first);
        for (const auto& p : features2) allFeatures.insert(p.first);

        double dotProduct = 0.0;
        double norm1 = 0.0, norm2 = 0.0;

        for (const auto& feat : allFeatures) {
            int v1 = features1[feat];
            int v2 = features2[feat];
            dotProduct += v1 * v2;
            norm1 += v1 * v1;
            norm2 += v2 * v2;
        }

        double featureSim = 0.5;
        if (norm1 > 0 && norm2 > 0) {
            featureSim = dotProduct / (sqrt(norm1) * sqrt(norm2));
        }

        // Combined similarity
        return 0.6 * structSim + 0.4 * featureSim;
    }
};

int main(int argc, char* argv[]) {
    string mode = "cheat";
    if (argc >= 2) {
        mode = argv[1];
    }

    // Auto-detect mode based on input if not specified
    // For cheat: single program
    // For anticheat: two programs with "endprogram" separator

    if (mode == "cheat") {
        // Transform program to evade plagiarism detection
        string program = readProgram(cin);
        Parser parser(program);
        auto exprs = parser.parseAll();

        Transformer transformer;
        auto transformed = transformer.transformProgram(exprs);

        for (const auto& expr : transformed) {
            expr->print(cout);
            cout << "\n";
        }
    } else if (mode == "anticheat") {
        // Compute similarity between two programs
        string program1 = readProgram(cin);
        string program2 = readProgram(cin);

        Parser parser1(program1);
        auto exprs1 = parser1.parseAll();

        Parser parser2(program2);
        auto exprs2 = parser2.parseAll();

        SimilarityChecker checker;
        double similarity = checker.computeSimilarity(exprs1, exprs2);

        cout << fixed;
        cout.precision(6);
        cout << similarity << endl;
    } else {
        cerr << "Unknown mode: " << mode << "\n";
        return 1;
    }

    return 0;
}
