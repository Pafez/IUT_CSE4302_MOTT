#pragma once

#include<sstream>
#include<vector>

enum template_t {
    OneReciept_OneAmount,
    ManyReciept_OneAmount,
    ManyReciept_ManyAmount
};

class Template1;

class Entry {
protected:
    int sender;
    std::vector<int> recipients;
    std::vector<double> values;
    std::string reference;
public : 
    Entry() {}
    Entry(std::vector<int> rs, std::vector<double> vs=std::vector<double>(0), std::string ref="");

    void setVal(std::vector<double> v);
    void setRef(std::string r);
    virtual std::string serialize() =0;

    static std::vector<Template1> linearizeEntry(Entry & _entry, template_t templateType);
};

class Template1: public Entry{          //1->1
    public:
    Template1(int receiver, double amount, std::string ref);
    std::string serialize();
};

class Template2 : public Entry {        //1->many but tamount/many per person
public:
    Template2(std::vector<int> rl, int ta, std::string ref);
    std::string serialize();
};

class Template3 : public Entry {        //1->many and different amounts
public:
    Template3(std::vector<int> rl, std::vector<double> al, std::string ref);
    std::string serialize();
};

