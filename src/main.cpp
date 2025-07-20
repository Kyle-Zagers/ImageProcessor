#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include "TGA.h"
using namespace std;

void task1()
{
    TGA_File layer1("./input/layer1.tga");
    TGA_File pattern1("./input/pattern1.tga");
    layer1.multiply(pattern1);
    layer1.TGA_Write("./output/part1.tga");
}

void task2()
{
    TGA_File layer2("./input/layer2.tga");
    TGA_File car("./input/car.tga");
    car.subtract(layer2);
    car.TGA_Write("./output/part2.tga");
}

void task3()
{
    TGA_File layer1("./input/layer1.tga");
    TGA_File pattern2("./input/pattern2.tga");
    layer1.multiply(pattern2);
    TGA_File text("./input/text.tga");
    layer1.screen(text);
    layer1.TGA_Write("./output/part3.tga");
}

void task4()
{
    TGA_File layer2("./input/layer2.tga");
    TGA_File circles("./input/circles.tga");
    layer2.multiply(circles);
    TGA_File pattern2("./input/pattern2.tga");
    layer2.subtract(pattern2);
    layer2.TGA_Write("./output/part4.tga");
}

void task5()
{
    TGA_File layer1("./input/layer1.tga");
    TGA_File pattern1("./input/pattern1.tga");
    layer1.overlay(pattern1);
    layer1.TGA_Write("./output/part5.tga");
}

void task6()
{
    TGA_File car("./input/car.tga");
    car.addgreen(200);
    car.TGA_Write("./output/part6.tga");
}

void task7()
{
    TGA_File car("./input/car.tga");
    car.scalered(4);
    car.scaleblue(0);
    car.TGA_Write("./output/part7.tga");
}

void task8()
{
    TGA_File car("./input/car.tga");
    car.onlyred().TGA_Write("./output/part8_r.tga");
    car.onlygreen().TGA_Write("./output/part8_g.tga");
    car.onlyblue().TGA_Write("./output/part8_b.tga");
}

void task9()
{
    TGA_File red("./input/layer_red.tga");
    TGA_File green("./input/layer_green.tga");
    TGA_File blue("./input/layer_blue.tga");
    red.combine(green, blue);
    red.TGA_Write("./output/part9.tga");
}

void task10()
{
    TGA_File text2("./input/text2.tga");
    text2.flip();
    text2.TGA_Write("./output/part10.tga");
}


int main(int argc, const char* argv[])
{
    vector<string> cla(argv, argv + argc);
    if (argc == 1 || (argc == 2 && cla[1] == "--help"))
    {
        cout << "Project 2: Image Processing, Spring 2023\n\nUsage:\n\t./project2.out [output] [firstImage] [method] [...]";
        return 0;
    }


    if (cla[1].length() < 5 || (cla[1].substr(cla[1].length() - 4, cla[1].length() - 1) != ".tga"))
    {
        cout << "Invalid file name.";
        return 0;
    }
    if (cla[2].length() < 5 || (cla[2].substr(cla[2].length() - 4, cla[2].length() - 1) != ".tga"))
    {
        cout << "Invalid file name.";
        return 0;
    }

    string output = cla[1];
    TGA_File trackingimage(cla[2]);

    // probably not the most efficient, but we just learned about function pointers so I gave it a shot.
    using method = void(TGA_File::*)(TGA_File);
    map<string, method> tgaMethods;
    tgaMethods["multiply"] = &TGA_File::multiply;
    tgaMethods["subtract"] = &TGA_File::subtract;
    tgaMethods["overlay"] = &TGA_File::overlay;
    tgaMethods["screen"] = &TGA_File::screen;

    using method2 = void(TGA_File::*)(TGA_File, TGA_File);
    map<string, method2> tga2Methods;
    tga2Methods["combine"] = &TGA_File::combine;

    using method3 = void(TGA_File::*)();
    map<string, method3> voidMethods;
    voidMethods["flip"] = &TGA_File::flip;

    using method4 = TGA_File(TGA_File::*)();
    map<string, method4> voidtgaMethods;
    voidtgaMethods["onlyred"] = &TGA_File::onlyred;
    voidtgaMethods["onlygreen"] = &TGA_File::onlygreen;
    voidtgaMethods["onlyblue"] = &TGA_File::onlyblue;

    using method5 = void(TGA_File::*)(int);
    map<string, method5> intMethods;
    intMethods["addred"] = &TGA_File::addred;
    intMethods["addgreen"] = &TGA_File::addgreen;
    intMethods["addblue"] = &TGA_File::addblue;
    intMethods["scalered"] = &TGA_File::scalered;
    intMethods["scalegreen"] = &TGA_File::scalegreen;
    intMethods["scaleblue"] = &TGA_File::scaleblue;

    for (int i = 3; i < cla.size(); i++)
    {
        if (tgaMethods.find(cla[i]) != tgaMethods.end())
        {
            if (i + 1 >= cla.size())
            {
                cout << "Missing argument.";
                return 0;
            }
            if (cla[i+1].length() < 5 || (cla[i+1].substr(cla[i+1].length() - 4, cla[i+1].length() - 1) != ".tga"))
            {
                cout << "Invalid argument, invalid file name.";
                return 0;
            }

            ifstream in(cla[i + 1], ios::binary);
            if (!in.is_open())
            {
                cout << "Invalid argument, file does not exist.";
                return 0;
            }
            in.close();

            TGA_File file(cla[i + 1]);
            (trackingimage.*tgaMethods[cla[i]])(file);
            i++;
        }
        else if (tga2Methods.find(cla[i]) != tga2Methods.end())
        {
            if ((i + 1 >= cla.size()) || (i+2 >= cla.size()))
            {
                cout << "Missing argument.";
                return 0;
            }
            if (cla[i + 1].length() < 5 || (cla[i + 1].substr(cla[i + 1].length() - 4, cla[i + 1].length() - 1) != ".tga"))
            {
                cout << "Invalid argument, invalid file name.";
                return 0;
            }
            if (cla[i + 2].length() < 5 || (cla[i + 2].substr(cla[i + 2].length() - 4, cla[i + 2].length() - 1) != ".tga"))
            {
                cout << "Invalid argument, invalid file name.";
                return 0;
            }

            ifstream in(cla[i + 1], ios::binary);
            ifstream in2(cla[i + 2], ios::binary);
            if (!in.is_open() || !in2.is_open())
            {
                cout << "Invalid argument, file does not exist.";
                return 0;
            }
            in.close();
            in2.close();

            TGA_File file(cla[i + 1]);
            TGA_File file2(cla[i + 2]);
            (trackingimage.*tga2Methods[cla[i]])(file, file2);
            i += 2;
        }
        else if (voidMethods.find(cla[i]) != voidMethods.end())
        {
            (trackingimage.*voidMethods[cla[i]])();
        }
        else if (voidtgaMethods.find(cla[i]) != voidtgaMethods.end())
        {
            trackingimage = (trackingimage.*voidtgaMethods[cla[i]])();
        }
        else if (intMethods.find(cla[i]) != intMethods.end())
        {
            if (i + 1 >= cla.size())
            {
                cout << "Missing argument.";
                return 0;
            }
            try { (trackingimage.*intMethods[cla[i]])(stoi(cla[i + 1])); }
            catch (const invalid_argument&) { cout << "Invalid argument, expected number."; return 0; }
            i++;
        }
        else
        {
            cout << "Invalid method name.";
            return 0;
        }
    }

    trackingimage.TGA_Write(output);


    //task1();
    //task2();
    //task3();
    //task4();
    //task5();
    //task6();
    //task7();
    //task8();
    //task9();
    //task10();

    return 0;
}