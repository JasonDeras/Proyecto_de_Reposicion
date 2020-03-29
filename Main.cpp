#include <ncurses.h>
#include <iostream>
#include <string>
#include <string.h>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <algorithm>
#include <sys/stat.h>

using namespace std;
string ruta="";
string usuario="home";
string machine="@vm-gauss:~$ ";
string inicio=usuario+""+machine+""+ruta;
int lineas=0;

string getstring(){

    string input;

    nocbreak();
    echo();

    int ch = getch();

    while ( ch != '\n' ){
        input.push_back( ch );
        ch = getch();
    }//fin del while

    return input;

}//metodo para conseguir los comandos

string ChangeMachine(string com){

	string newMach="";

	for (int i = 14; i < com.size(); ++i){
		newMach+=com[i];

	}//fin del for

	return newMach;

}//fin del metodo para cambiar la maquina

string ChangeUser(string com){

	string newuse="";

	for (int i = 11; i < com.size(); ++i){
		newuse+=com[i];
	}//fin del for

	return newuse;

}//fin del metodo para cambiar el usuario

void file(string com){

	string nombre;
	string commando;

	for (int i = 5; i < com.size(); ++i){
		nombre+=com[i];
	}

	if(nombre.find(".txt")!= std::string::npos&& ruta==""){
		ofstream file("./"+nombre,ios::out);
	}else if(nombre.find(".txt")!= std::string::npos&& ruta!=""){
		commando="cd >> "+ruta+"/"+nombre;
		system(commando.c_str());
	}

}//fin del metodo para escribir un archivo

void write(string com){
	
	string nombre;
	string palabra;
	int espacios=0;

	for (int i = 0; i < com.size(); ++i){
		
		if(com[i]==' '){
			espacios++;
		}

	}//fin del for que lee lo que ingreso 

	if(espacios==3){

		espacios=0;

		for (int i = 6; i < com.size(); ++i){

			if(com[i]==' '){
				break;
			}else {
				nombre+=com[i];
			}
		}//fin del for para sacar la ruta

		for (int i = 6; i < com.size(); ++i){
			
			if(com[i]==' '){
				espacios++;
			}

			if(espacios==1){
				palabra+=com[i+1];
			}
		}

		if(ruta!=""){
			ofstream escribir;
			escribir.open("./"+ruta+"/"+nombre,ios::app);
			if(escribir.is_open()){
				escribir<<palabra<<endl;
			}else {
				exit(1);
			}
			escribir.close();	
		}else if(ruta==""){
			ofstream escribir;
			escribir.open("./"+nombre,ios::app);
		
			if(escribir.is_open()){
				escribir<<palabra<<endl;
			}else {
				exit(1);
			}
			escribir.close();
		}
	}else{
		addstr("Error fromato del comando no es valido \n");
	}

}//fin del metodo para escribir en el archivo

void read(string com){

	string texto;
	fstream leer;
	string linea;
	bool leido=false;
	for (int i = 5; i < com.size(); ++i){
		texto+=com[i];	
	}//fin del for para sacar la ruta del archivo

	if(ruta!=""){
		leer.open("./"+ruta+"/"+texto);
		if(leer.is_open()){
			while(!leer.eof()){
				getline(leer,linea);
				addstr(linea.c_str());
				addstr("\n");
			}//fin del while 
		}else {
			addstr("El archivo no se puedo abririr\n");
		}//fin del if
		leer.close();
		leido=true;	
	}else if(ruta==""){
		
		leer.open("./"+texto);
		
		if(leer.is_open()){
			while(!leer.eof()){
				getline(leer,linea);
				addstr(linea.c_str());
				addstr("\n");
			}//fin del while 
			
		}else {
			addstr("El archivo no se puedo abririr\n");
		}//fin del if
		leer.close();
		leido=true;	
	}//fin del if para leer el archivo en carpeta o fuera de la carpeta
	
	if(leido==false){
	}else if(leido==true){
		lineas++;
	}//mueve el cursor a la linea correcta

}//fin del metodo para leer el archivo

void crear_carpeta(string com){

	string nombre;
	int resultado;

	for (int i = 6; i < com.size(); ++i){
		nombre+=com[i];
	}

	resultado=mkdir(nombre.c_str(),0777);

}//fin del metodo para crear la carpeta

void borrar_carpeta(string com){
	
	string nombre;
	string comando;

	for (int i = 4; i < com.size(); ++i){
		nombre+=com[i];
	}
	
	comando="rmdir "+nombre;

	system(comando.c_str());

}//fin del metodo para borrar la carpeta	

string ingresar_carpeta(string com){
	
	string nombre;
	string comando;
	
	for (int i = 3; i < com.size(); ++i){
		nombre+=com[i];
	}

	comando="cd "+nombre;
	ruta=nombre;
	system(comando.c_str());
	
	return nombre;

}//fin del metodo para ingresar a la carpeta

int main() {

 	initscr();
 	start_color();
 	init_pair(1, COLOR_YELLOW, COLOR_BLACK);
 	init_pair(2, COLOR_BLUE,COLOR_BLACK);
 	init_pair(3, COLOR_YELLOW,COLOR_RED);
 	init_pair(4,COLOR_YELLOW,COLOR_WHITE);
	attron(COLOR_PAIR(1));
	string acces;
	string color;

	while(true){

		if(ruta==""){
			addstr(inicio.c_str());
		}else if(ruta!=""){
			usuario="home";
			machine="@vm-gauss:";
			inicio=usuario+""+machine+"/"+ruta+"$";
			addstr(inicio.c_str());
		}

		move(lineas,inicio.size()+1);

		string comando=getstring();
		
		if(comando.find("changeMachine")!= std::string::npos){
			machine="@"+ChangeMachine(comando)+":~$";
			inicio=usuario+""+machine;
		}//if de la maquina

		if(comando.find("changeUser")!= std::string::npos){
			usuario=ChangeUser(comando);
			inicio=usuario+""+machine;
		}//if de el usuario

		if(comando.find("changeColor")!= std::string::npos){

			for (int i = 12; i < comando.size(); ++i){
				color+=comando[i];
			}//fin del for

			std::for_each(color.begin(), color.end(), [](char & x) {
				x = ::tolower(x);
			});//pasa el color a minsuculas

		}//if del color

		if(color=="white"){
			attroff(COLOR_PAIR(1));
			attroff(COLOR_PAIR(3));
			attron(COLOR_PAIR(4));
			color="";
		}else if(color=="red"){
			attroff(COLOR_PAIR(1));
			attroff(COLOR_PAIR(4));
			attron(COLOR_PAIR(3));
			color="";
		}else if(color=="black"){
			attroff(COLOR_PAIR(4));
			attroff(COLOR_PAIR(3));
			attron(COLOR_PAIR(1));
			color="";
		}//fin del cambio de color 

		if(comando.find("file")!= std::string::npos){
			file(comando);
		}//if de la creacion de un archivo

		if(comando.find("write")!= std::string::npos){
			write(comando);
		}//if de la escritura de un archivo


		if(comando.find("read")!= std::string::npos){
			read(comando);
			lineas++;
		}//if de la lectura de un archivo

		if(comando.find("mkdir")!= std::string::npos){
			crear_carpeta(comando);
		}//if de la creacion de una carpeta


		if(comando.find("del")!= std::string::npos){
			borrar_carpeta(comando);
		}//if de la eliminacion de una carpeta


		if(comando.find("cd")!= std::string::npos){
			inicio+=ingresar_carpeta(comando);
			attron(COLOR_PAIR(2));
		}//if de la eliminacion de una carpeta

		if(comando.find("ls")!= std::string::npos&& ruta!=" "){
			acces="dir "+ruta;
			system(acces.c_str());
			lineas=0;
		}//if del ls con la ruta

		if(comando.find("cd/")!= std::string::npos&& ruta!=" "){
			inicio=usuario+""+machine;
			attron(COLOR_PAIR(1));
			system("cd");
		}//if de regresar a la carpeta anterior

		if(comando.find("clear")!= std::string::npos){
			clear();
			lineas=0;	
		}else{
			lineas++;
		}//if para lineasrolar los comandos de clear de ncurses
		
		if(comando=="exit"){
			endwin();
			exit(1);
		}//if del exit

	}//fin del while repetitivo

	endwin();

 	return 0;

}//fin del main