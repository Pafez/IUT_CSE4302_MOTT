#include "./Entry.h"
#include "./Session.h"

Entry::Entry(std::vector<int> rs, std::vector<double> vs, std::string ref){
    Account& temp = Session::getAccount();
    sender = temp.getID();
    recipients = rs;
    values = vs;
    reference = ref;
}
void Entry::setVal(std::vector<double> v){values = v;}
void Entry::setRef(std::string r){reference = r;}

Template1::Template1(int receiver, double amount, std::string ref): Entry(std::vector<int>(1,receiver), std::vector<double>(1,amount), ref){}

std::string Template1::serialize(){
    std::string line;

    line = "1|" + std::to_string(sender) + '|' + std::to_string(recipients[0]) + '|' + std::to_string(values[0]) + '|' + reference;
    return line;
}

Template2::Template2(std::vector<int> rl, int ta, std::string ref): Entry(rl){
    int number = rl.size();
    double amounts = (double)ta/number;
    std::vector<double> temp;
    temp.push_back(amounts);

    setVal(temp);
    setRef(ref);
}

std::string Template2::serialize(){
    int number = recipients.size();
    std::string line;
    line = "2|" + std::to_string(sender) + '|';

    line+= std::to_string(number) + '|';
    for(int i=0; i<number-1; i++){line+=std::to_string(recipients[i]) + ',';}
    line+=std::to_string(recipients[number-1]) + '|';

    line+=std::to_string(values[0])  + '|' + reference;
    return line;
}

Template3::Template3(std::vector<int> rl, std::vector<double> al, std::string ref): Entry(rl,al,ref){}

std::string Template3::serialize(){
    int number = recipients.size();
    std::string line;
    line = "3|" + std::to_string(sender) + '|';
    
    line+= std::to_string(number) + '|';
    for(int i=0; i<number-1; i++){line+=std::to_string(recipients[i]) + ',';}
    line+=std::to_string(recipients[number-1]) + '|';
    
    for(int i=0; i<number-1; i++){line+=std::to_string(values[i]) + ',';}
    line+=std::to_string(values[number-1]) + '|';
    
    line+=reference;
    return line;
}

std::vector<Template1> Entry::linearizeEntry(Entry & _entry, template_t templateType) {
    std::vector<Template1> dummy;
    int totalReceipt  = _entry.recipients.size();
    
    switch (templateType)
    {
    case OneReciept_OneAmount:
        dummy.push_back(Template1(_entry.recipients[0], _entry.values[0], _entry.reference));
        break;
    
    case ManyReciept_OneAmount:
        for (int i = 0; i < totalReceipt; i++) {
            dummy.push_back(Template1(_entry.recipients[i], _entry.values[0], _entry.reference));
        }
        break;
    
    case ManyReciept_ManyAmount:
        for (int i = 0; i < totalReceipt; i++) {
            dummy.push_back(Template1(_entry.recipients[i], _entry.values[i], _entry.reference));
        }
        break;
    
    default:
        break;
    }

    return dummy;
}
