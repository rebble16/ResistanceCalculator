// ResistanceCalculator.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include<vector>
enum mode { SEQ = 0, PAR = 1 };
class resist
{
protected:
    std::vector<double> data;//содержит информацию о сопротивлениях резисторов
    int size;//количество резисторов(можно было бы использовать функцию size() из vector,но я решил оставить это поле,т.к. изначально планировал другую реализацию
    mode Mode;//отвечает за соединение резисторов в сегменте
    double res;//итоговое сопротивление сегмента
public:
    resist() :data(0.0),size(0), Mode(SEQ), res(0.0) {}//базированный конструктор
    resist(std::vector<double>& d, int& s, mode& a) :data(d), size(s), Mode(a) {}//конструктор для инициализации данных в main()
    void showMeInput()const;//для вывода введеных данных
    ~resist();//лишние буквы
    void calculate();//вычисляет итоговое сопротивление сегмента
    void showMeOutput()const;//для вывода результата
    mode getMode()const { return Mode; }//для доступа к Mode в наследуемом классе Circuit
    double getRes()const { return res; }//для доступа к res в наследуемом классе Circuit
};
class Circuit :protected resist
{
private:
    int counter;//счетчик сегментов (считает количество сегментов в виде резисторов с определенным соединением,counter=количество узлов-1)
    resist* p_circuit;//указывает на сегменты цепи из main(),объявленных в виде массива
    double result;//итоговый результат расчета цепи
public:
    Circuit(int c=0, resist* pc = nullptr,double r=0) :counter(c),p_circuit(pc),result(r){}//базированный конструктор,ну и не базированный
    ~Circuit();//лишние буквы
    void CalculateCircuit();//функция для сложения всех сегментов цепи
    friend std::ostream& operator<<(std::ostream& os, Circuit& obj);//функция выводящая итоговый результат расчета цепи
};
    int main()
    {
        const int count = 3;//объявление количества сегментов в цепи
        int resistors[count] = { 2, 2, 3 };//объявление количества резисторов в каждом сегменте
        mode modes[count] = { PAR, SEQ,PAR };//объявление вида связи сопротивлений в каждом сегменте
        resist nodes[count];//объявление сегментов цепи
        double input_resist;//сопротивление резистора
  
        for (int i = 0; i < count; i++)
        {
            std::cout << "ENTER THE DATA FOR THE " << i + 1 << " SEGMENT" << std::endl;
            std::vector<double> resistance;//сопротивление резисторов для одного сегмента(каждый раз при итерации объявляется новая переменная resistance
            for (int j = 0; j < resistors[i]; j++)//ввод данных о сопротивлении каждого резистора в сегменте
            {
                std::cout << "Enter resistance of " << j + 1 << " resistor in OM:";
                std::cin >> input_resist;
                resistance.push_back(input_resist);
                while (std::cin.get() != '\n')continue;
                std::cin.clear();
            }
            nodes[i] = resist(resistance, resistors[i], modes[i]);//передача данных о сегменте
     
            nodes[i].showMeInput();//вывод переданных данных
            nodes[i].calculate();//расчет сопротивления сегмента
            nodes[i].showMeOutput();//вывод сопротивления сегмента
        }
        Circuit circuit(count, nodes, 0.0);  //передача данных о количества сегментов,сопротивление сегментов и обнуление итогового результата
        circuit.CalculateCircuit();//расчет итогового сопротивления цепи
        std::cout << circuit << std::endl;//вывод итогового результата
    return 0;
}

    void resist::showMeInput() const
    {
        std::cout << "Your mode is:";
        switch (Mode)
        {
        case SEQ:std::cout << "SEQUENT curcuit" << std::endl;
            break;
        case PAR:std::cout << "PARALLEL curcuit" << std::endl;
            break;
        default:std::cout << "Error!" << std::endl;
        }
        for (int i = 0; i < size; i++)
            std::cout << "Resistance of R" << i + 1 << "=" << data[i] << " OM" << std::endl;


    }
resist::~resist()
{

}
void resist::calculate()
{
    double result=0;
    double conductivity = 0.0;
    if (Mode == SEQ)
    {
        for (int i = 0; i < size; i++)result += data[i];
        res = result;
    }
    else if (Mode == PAR)
    {
        for (int i = 0; i < size; i++)
        {
            conductivity += 1.0 / data[i];
        }
        res = 1.0 / conductivity;
    }
}

void resist::showMeOutput() const
{
    if (Mode == SEQ)
        std::cout << "Common resistance of section with sequent link of resistors is " << res << " OM" << std::endl;
    else std::cout<< "Common resistance of section with parallel link of resistors is " << res << " OM" << std::endl;
}

Circuit::~Circuit()
{
    
}

void Circuit::CalculateCircuit()
{
    double res=0.0;
    for (int i = 0; i < counter; i++)
    {
        res = p_circuit[i].getRes();
       result += res;
   }
}

std::ostream& operator<<(std::ostream& os, Circuit& obj)
{
    os << "The common resistance of the whole curcuit is " << obj.result << " OM" << std::endl;
    return os;
}
