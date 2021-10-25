#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>
#include <set>
#include <string>
#define EPSILON 'e'

using namespace std;

struct data
{
    string nombre_estado;
    bool es_inicial;
    bool es_final;
    string conexiones;

};
typedef struct data Data_Nodo;

struct struct_nodo
{
    Data_Nodo Datos;
    struct struct_nodo *next;
    struct struct_nodo *conexiones;
    struct struct_nodo *conexiones_final;

};
typedef struct struct_nodo Nodo;

Nodo* crear_nodo(string, string);
void formato(string &x);
void leer_estado(string &x, ifstream &FILE);
void leer_conexion(string &x, ifstream &FILE, string &y);
int imprimir_grafo(Nodo*);
void imprimir_nodo(Nodo);
int inicializar_lista(Nodo**, Nodo**);
int insertar_lista_principio(Nodo**, Nodo*);
int insertar_lista_medio(Nodo*, Nodo*, string);
int insertar_lista_final(Nodo**, Nodo**, Nodo*);
int borrar_lista(Nodo *, Nodo**, Nodo**);
int borrar_grafo(Nodo *, Nodo**, Nodo**);
void obtener_caracteres(string &x, Nodo*);
Nodo* buscar_nodo(string, Nodo*);
void buscar_total_rutas(Nodo*, string, string, vector<string>&, vector<string>, vector<vector<string>>&);
int obtener_ruta_menor(vector<vector<string>>);

int main()
{
    int opcion = 0;
    Nodo* head = NULL;
    Nodo* last = NULL;

    do
    {
        cout << "Bienvenido al sistema de grafos\n";
        cout << "Seleccione una opcion : \n";
        cout << "1. Inicializar un grafo a traves de un archivo.\n";
        cout << "2. Imprimir grafo.\n";
        cout << "3. Borrar grafo.\n";
        cout << "4. Buscar todas las rutas posibles para llegar de un nodo a otro.\n";
        cout << "5. Encontrar la ruta mas corta de un nodo a otro.\n";
        cout << "0. Salir.\n";

        cin >> opcion;
        switch(opcion)
        {
            case 1:
            {
                    if(head != NULL)
                    {
                        cout << "Ya existe un grafo cargado\n";
                        break;
                    }
                    head = NULL;
                    last = NULL;
                    int x = inicializar_lista(&head, &last);
                    if(x == 1)
                        cout << "Grafo creado exitosamente.\n";
                    else if(x == -1)
                    {
                        cout << "Archivo no encontrado\n";
                        break;
                    }
                    else if(x == -2)
                    {
                        cout << "AVISO: El grafo no tenia conexiones\n";
                        break;
                    }
                    string caracteres = "";
                    obtener_caracteres(caracteres, head);

                    set<char>s(caracteres.begin(), caracteres.end());

                    int i = 0;
                    for(char x : s)
                        caracteres[i++] = x;
                    caracteres[i] = '\0';

                    cout << "Final: " << caracteres;

            }
            break;

            case 2:
                if(imprimir_grafo(head) == -1)
                    cout << "El grafo aun no se ha introducido o no tiene nada.\n";
            break;
            case 3:
                if(borrar_grafo(head, &head, &last) == -1)
                    cout << "El grafo aun no se ha introducido o no tiene nada.\n";
                else
                    cout << "Grafo borrado correctamente.\n";
            break;
            case 4:
            case 5:
            {
                if(head == NULL)
                {
                    cout << "No se ha introducido ningun grafo.\n";
                    break;
                }
                cout << "Introduce tu nodo origen: ";
                string nombre_origen;
                cin >> nombre_origen;
                cout << "Introduce tu nodo destino: ";
                string nombre_destino;
                cin >> nombre_destino;
                Nodo* nodo_origen = buscar_nodo(nombre_origen, head);
                Nodo* nodo_destino = buscar_nodo(nombre_destino, head);
                if(nodo_origen == NULL)
                {
                    cout << "No se encontro el nodo origen.\n";
                    break;
                }
                else if(nodo_destino == NULL)
                {
                    cout << "No se encontro el nodo destino.\n";
                    break;
                }
                vector<string> visitados;
                vector<string> ruta;
                vector<vector<string>> rutas;
                buscar_total_rutas(head, nombre_origen, nombre_destino, visitados, ruta, rutas);
                if(rutas.size() == 0)
                {
                    cout << "No hay rutas disponibles para llegar a ese nodo desde ese destino.\n";
                    break;
                }
                if(opcion == 4)
                    for(int i = 0; i < rutas.size(); ++i)
                    {
                        printf("Ruta %3d: ", i + 1);
                        for(int j = 0; j < rutas[i].size(); ++j)
                            if(j != rutas[i].size() - 1)
                                cout << rutas[i][j] << " -> ";
                            else
                                cout << rutas[i][j];
                        cout << "\n";
                    }
                else
                {
                    int pos = obtener_ruta_menor(rutas);
                    cout << "La ruta(s) mas corta(s) encontrada(s) fue(fueron): \n";
                    for(int i = 0; i < rutas.size(); ++i)
                        if(rutas[i].size() == pos)
                        {
                            for(int j = 0; j < rutas[i].size(); ++j)
                                if(j != rutas[i].size() - 1)
                                    cout << rutas[i][j] << " -> ";
                                else
                                    cout << rutas[i][j];
                            cout << "\n";
                        }
                }
            }
            break;
            default:
                cout << "Opcion incorrecta\n";
            break;
            case 0:
                cout << "Gracias por usar\n";
            break;
        }
        system("pause");
        system("cls");

    }while(opcion != 0);


    return 0;
}

void imprimir_nodo(Nodo nodo)
{
    cout << "Nombre del nodo: " << nodo.Datos.nombre_estado << "\n";
    if(nodo.Datos.es_inicial)
        cout << "Es nodo inicial.\n";
    else if(nodo.Datos.es_final)
        cout << "Es nodo final.\n";
    //cout << "Next: " << nodo.next << "\n";
    //cout << "Conexiones: " << nodo.conexiones << "\n";
    cout << "Sus conexiones son: " << "\n";
    Nodo *p = nodo.conexiones;
    if(p == NULL)
        cout << "Este nodo no tiene conexiones.\n";
    else
    {
        while(p != NULL)
        {
            cout << "Nombre del nodo: " << p -> Datos.nombre_estado << "\n";
            for(char c : p->Datos.conexiones)
            {
                cout << "Caracter(es) de transicion: " << c <<"\n";
            }
            p = p -> next;
        }
    }
    cout << "\n";

}

void formato(string &x)
{
    for(char c : x)
        c = tolower(c);
    x[0] = toupper(x[0]);
}

Nodo* crear_nodo(string aux, string conexiones)
{
    Nodo* nodo = new Nodo;
    nodo -> Datos.nombre_estado = aux;
    nodo -> Datos.es_inicial = false;
    nodo -> Datos.es_final = false;
    nodo -> next = NULL;
    nodo -> conexiones = NULL;
    nodo -> Datos.conexiones = conexiones;
    return nodo;
}

int imprimir_grafo(Nodo* head)
{
    Nodo* p = head;
    if(p == NULL)
        return -1;
    while(p != NULL)
    {
        //cout << "Direccion actual: " << p << "\n";
        imprimir_nodo(*p);
        p = p -> next;
    }
    return 0;
}

int insertar_lista_final(Nodo** head, Nodo** last, Nodo *nodo)
{
    if(*head == NULL)
    {
        *(head) = nodo;
        **head = *nodo;
        *last = *head;
        **last = **head;
    }
    else if(*head == *last)
    {
        (*head) -> next = nodo;
        *last = nodo;
        **last = *nodo;
    }
    else
    {
        (*last) -> next = nodo;
        *last = nodo;
        **last = *nodo;
    }
}

void leer_estado(string &x, ifstream &FILE)
{
    char c;
    FILE.get(c);
    if(c == '\n')
        FILE.get(c);
    while(c != '-' && c != '\n')
    {
        x += c;
        FILE.get(c);
        if(FILE.eof())
            return;
    }
}

void leer_conexion(string &x, ifstream &FILE, string&con)
{
    char c;
    FILE.get(c);
    if(c == '\n')
        FILE.get(c);
    while(c != '-' && c != '\n')
    {
        x += c;
        FILE.get(c);
        if(c == ',')
        {
            FILE.get(c);
            while(c != '\n' && FILE.eof() != 1)
            {
                con += c;
                FILE.get(c);
            }
            break;
        }
        if(FILE.eof())
            return;
    }
}

int inicializar_lista(Nodo** head, Nodo** last)
{

    ifstream FILE;
    string ruta;
    cout << "Escribe la ruta del archivo que contiene al grafo: ";
    cin >> ruta;
    FILE.open(ruta, ios::in);
    if(!FILE.is_open())
        return -1;

    //Primer renglón: estados
    char c = '\0';
    string sr = "";
    while(sr != "-1")
    {
        FILE.get(c);
        if(sr == "-1")
            break;
        if(c != ',')
            sr += c;
        else
        {
            formato(sr);
            Nodo* aux_nodo = crear_nodo(sr, "\0");
            insertar_lista_final(head, last, aux_nodo);
            sr = "";
        }
    }

    (*head) -> Datos.es_inicial = true;

    //Segundo renglón: estados finales
    sr = "";
    FILE.get(c);

    while(sr != "-1")
    {
        FILE.get(c);
        if(sr == "-1")
            break;
        if(c != ',')
            sr += c;
        else
        {
            formato(sr);
            Nodo* aux_nodo = buscar_nodo(sr, *head);
            aux_nodo -> Datos.es_final = true;
            sr = "";
        }
    }

    //Tercer renglón: Conexiones;

    if(FILE.eof())
        return -2;

    string nodo_1;
    string nodo_2;

    Nodo *p = *head;
    Nodo* aux = NULL;
    Nodo* aux_2 = NULL;
    Nodo* aux_nodo = NULL;
    string aux_conexion = "\0";

    while(!FILE.eof())
    {


        leer_estado(nodo_1, FILE);
        leer_conexion(nodo_2, FILE, aux_conexion);

        formato(nodo_1);
        formato(nodo_2);
        aux = buscar_nodo(nodo_1, *head);
        aux_2 = buscar_nodo(nodo_2, *head);
        if(aux == NULL || aux_2 == NULL)
        {
            cout << "Error en el nombre de las conexiones\n";
            cout << "Se procedera a cerrar el programa. Revise su archivo.\n";
            exit(0);
        }
        aux_nodo = crear_nodo(nodo_2, aux_conexion);
        insertar_lista_final(&(aux -> conexiones), &(aux -> conexiones_final), aux_nodo);
        nodo_1 = nodo_2 = aux_conexion = "";
        leer_estado(nodo_1, FILE);
        leer_conexion(nodo_2, FILE, aux_conexion);
        formato(nodo_1);
        formato(nodo_2);
    }

    aux = buscar_nodo(nodo_1, *head);
    aux_2 = buscar_nodo(nodo_2, *head);
    if(aux == NULL || aux_2 == NULL)
    {
        cout << "Error en el nombre de las conexiones\n";
        cout << "Se procedera a cerrar el programa. Revise su archivo.\n";
        exit(0);
    }
    aux_nodo = crear_nodo(nodo_2, aux_conexion);
    insertar_lista_final(&(aux -> conexiones), &(aux -> conexiones_final), aux_nodo);

    return 1;
}

Nodo* buscar_nodo(string nombre, Nodo* head)
{
    Nodo* p = head;
    while(p != NULL)
    {
        if(p ->Datos.nombre_estado == nombre)
            return p;
        p = p -> next;
    }
    return NULL;
}

int borrar_lista(Nodo *p, Nodo** head, Nodo** last)
{
    if(p == NULL)
        return -1;
    if(p -> next == NULL)
    {
        *head = NULL;
        *last = NULL;
        return 0;
    }
    else
    {
        borrar_lista(p -> next, head, last);
        delete p;
        p = NULL;
    }
}

int borrar_grafo(Nodo *p, Nodo** head, Nodo** last)
{
    if(p == NULL)
        return -1;
    while(p != NULL)
    {
        borrar_lista(p ->conexiones, &(p -> conexiones), &(p -> conexiones_final));
        p = p -> next;
    }
    p = *head;
    borrar_lista(p, head, last);
    return 1;

}

void buscar_total_rutas(Nodo* head, string origen, string destino, vector<string> &visitados, vector<string> ruta,
                        vector<vector<string>>&rutas)
{
    visitados.push_back(origen);
    ruta.push_back(origen);
    if(origen == destino)
        rutas.push_back(ruta);
    else
    {
        Nodo* p = buscar_nodo(origen, head);
        Nodo* q = p ->conexiones;

        while(q != NULL)
        {

            bool esta_visitado = find(visitados.begin(), visitados.end(), q->Datos.nombre_estado) != visitados.end();
            if(!esta_visitado)
                buscar_total_rutas(head, q->Datos.nombre_estado, destino, visitados, ruta, rutas);
            q = q -> next;
        }
    }

    auto i = find(visitados.begin(), visitados.end(), origen);
    visitados.erase(i);

    return;

}

int obtener_ruta_menor(vector<vector<string>> rutas)
{
    int menor = rutas[0].size();

    for(int i = 1; i < rutas.size(); ++i)
        if(menor > rutas[i].size())
            menor = rutas[i].size();

    return menor;
}

void obtener_caracteres(string &x, Nodo* head)
{
    Nodo *p = head;
    while(p != NULL)
    {
        Nodo *q = p ->conexiones;
        while(q != NULL)
        {
            x += q -> Datos.conexiones;
            q = q -> next;
        }
        p = p -> next;
    }

}
