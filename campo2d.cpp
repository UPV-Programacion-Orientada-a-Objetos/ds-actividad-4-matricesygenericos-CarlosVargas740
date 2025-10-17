#include <iostream>
#include <cmath>

template <typename T>
class Campo2D {
private:
    T **datos;
    int filas;
    int columnas;

public:
    Campo2D(int f, int c);
    ~Campo2D();
    
    void redimensionar(int nuevaF, int nuevaC);
    void setValor(int f, int c, T valor);
    T getValor(int f, int c) const;
    T calcularGradientePromedio(int inicioF, int finF, int inicioC, int finC);
    void mostrarGrid();
    int getFilas() const { return filas; }
    int getColumnas() const { return columnas; }
};

template <typename T>
Campo2D<T>::Campo2D(int f, int c) : filas(f), columnas(c)
{
    std::cout << "Creando Grid de " << filas << "x" << columnas << "...\n";
    
    datos = new T*[filas];
    for (int i = 0; i < filas; i++) {
        datos[i] = new T[columnas];
        for (int j = 0; j < columnas; j++) {
            datos[i][j] = 0;
        }
    }
}

template <typename T>
Campo2D<T>::~Campo2D()
{
    std::cout << "Destructor invocado. Liberando memoria de la Matriz 2D...\n";
    for (int i = 0; i < filas; i++) {
        delete[] datos[i];
    }
    delete[] datos;
}

template <typename T>
void Campo2D<T>::redimensionar(int nuevaF, int nuevaC)
{
    std::cout << "\nRedimensionando Grid a " << nuevaF << "x" << nuevaC << "...\n";
    
    T **nuevosDatos = new T*[nuevaF];
    for (int i = 0; i < nuevaF; i++) {
        nuevosDatos[i] = new T[nuevaC];
        for (int j = 0; j < nuevaC; j++) {
            nuevosDatos[i][j] = 0;
        }
    }
    
    int minFilas = (filas < nuevaF) ? filas : nuevaF;
    int minCols = (columnas < nuevaC) ? columnas : nuevaC;
    
    for (int i = 0; i < minFilas; i++) {
        for (int j = 0; j < minCols; j++) {
            nuevosDatos[i][j] = datos[i][j];
        }
    }
    
    for (int i = 0; i < filas; i++) {
        delete[] datos[i];
    }
    delete[] datos;
    
    datos = nuevosDatos;
    filas = nuevaF;
    columnas = nuevaC;
    
    std::cout << "Datos copiados. Memoria antigua liberada.\n";
}

template <typename T>
void Campo2D<T>::setValor(int f, int c, T valor)
{
    if (f >= 0 && f < filas && c >= 0 && c < columnas) {
        datos[f][c] = valor;
    } else {
        std::cout << "Indices fuera de rango.\n";
    }
}

template <typename T>
T Campo2D<T>::getValor(int f, int c) const
{
    if (f >= 0 && f < filas && c >= 0 && c < columnas) {
        return datos[f][c];
    }
    std::cout << "Indices fuera de rango.\n";
    return 0;
}

template <typename T>
T Campo2D<T>::calcularGradientePromedio(int inicioF, int finF, int inicioC, int finC)
{
    if (inicioF < 0 || finF >= filas || inicioC < 0 || finC >= columnas) {
        std::cout << "Region fuera de limites.\n";
        return 0;
    }
    
    if (inicioF > finF || inicioC > finC) {
        std::cout << "Indices invalidos.\n";
        return 0;
    }
    
    std::cout << "\nCalculando Gradiente Promedio en la region [" 
              << inicioF << "," << finF << "]x[" << inicioC << "," << finC << "]...\n";
    
    T sumaGradientes = 0;
    int conteo = 0;
    
    for (int i = inicioF; i <= finF; i++) {
        for (int j = inicioC; j <= finC; j++) {
            T difTotal = 0;
            int vecinos = 0;
            
            if (i + 1 <= finF) {
                T dif = datos[i+1][j] - datos[i][j];
                difTotal += (dif < 0) ? -dif : dif;
                vecinos++;
            }
            
            if (j + 1 <= finC) {
                T dif = datos[i][j+1] - datos[i][j];
                difTotal += (dif < 0) ? -dif : dif;
                vecinos++;
            }
            
            if (vecinos > 0) {
                sumaGradientes += difTotal / vecinos;
                conteo++;
            }
        }
    }
    
    return (conteo > 0) ? (sumaGradientes / conteo) : 0;
}

template <typename T>
void Campo2D<T>::mostrarGrid()
{
    std::cout << "\nGrid Actual:\n";
    for (int i = 0; i < filas; i++) {
        std::cout << "| ";
        for (int j = 0; j < columnas; j++) {
            std::cout << datos[i][j] << " | ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

template <typename T>
void menuPrincipal(Campo2D<T> &campo)
{
    int op;
    do {
        std::cout << "\nMenu\n";
        std::cout << "1. Mostrar Grid\n";
        std::cout << "2. Establecer Valor\n";
        std::cout << "3. Calcular Gradiente Promedio\n";
        std::cout << "4. Redimensionar\n";
        std::cout << "5. Salir\n";
        std::cout << "Opcion: ";
        std::cin >> op;
        
        switch(op) {
            case 1:
                campo.mostrarGrid();
                break;
                
            case 2: {
                int f, c;
                T val;
                std::cout << "Fila: ";
                std::cin >> f;
                std::cout << "Columna: ";
                std::cin >> c;
                std::cout << "Valor: ";
                std::cin >> val;
                campo.setValor(f, c, val);
                std::cout << "Valor establecido.\n";
                break;
            }
            
            case 3: {
                int inicioF, finF, inicioC, finC;
                std::cout << "Fila Inicial: ";
                std::cin >> inicioF;
                std::cout << "Fila Final: ";
                std::cin >> finF;
                std::cout << "Columna Inicial: ";
                std::cin >> inicioC;
                std::cout << "Columna Final: ";
                std::cin >> finC;
                
                T gradiente = campo.calcularGradientePromedio(inicioF, finF, inicioC, finC);
                std::cout << "Gradiente Promedio calculado: " << gradiente 
                          << " unidades/metro.\n";
                break;
            }
            
            case 4: {
                int nf, nc;
                std::cout << "Nueva cantidad de filas: ";
                std::cin >> nf;
                std::cout << "Nueva cantidad de columnas: ";
                std::cin >> nc;
                campo.redimensionar(nf, nc);
                break;
            }
            
            case 5:
                std::cout << "Saliendo...\n";
                break;
                
            default:
                std::cout << "Opcion invalida.\n";
        }
    } while(op != 5);
}

int main()
{
    std::cout << "Simulador Generico de Campo 2D \n\n";
    
    int tipo;
    std::cout << "Selecciona tipo de dato:\n";
    std::cout << "1. INT\n";
    std::cout << "2. FLOAT\n";
    std::cout << "3. DOUBLE\n";
    std::cout << "Opcion: ";
    std::cin >> tipo;
    
    int f, c;
    std::cout << "\nIngresa dimensiones iniciales:\n";
    std::cout << "Filas: ";
    std::cin >> f;
    std::cout << "Columnas: ";
    std::cin >> c;
    
    if (tipo == 1) {
        std::cout << "\n>> Inicializando Campo (Tipo INT) <<\n";
        Campo2D<int> campo(f, c);
        
        std::cout << "Estableciendo valores iniciales...\n";
        for (int i = 0; i < campo.getFilas() && i < 3; i++) {
            for (int j = 0; j < campo.getColumnas() && j < 3; j++) {
                campo.setValor(i, j, 10 + i * 3 + j);
            }
        }
        
        menuPrincipal(campo);
        
    } else if (tipo == 2) {
        std::cout << "\n>> Inicializando Campo Gravitatorio (Tipo FLOAT) <<\n";
        Campo2D<float> campo(f, c);
        
        std::cout << "Estableciendo valores iniciales...\n";
        for (int i = 0; i < campo.getFilas() && i < 3; i++) {
            for (int j = 0; j < campo.getColumnas() && j < 3; j++) {
                campo.setValor(i, j, 10.0f - i * 2.0f - j * 1.5f);
            }
        }
        
        menuPrincipal(campo);
        
    } else if (tipo == 3) {
        std::cout << "\n>> Inicializando Campo (Tipo DOUBLE) <<\n";
        Campo2D<double> campo(f, c);
        
        std::cout << "Estableciendo valores iniciales...\n";
        for (int i = 0; i < campo.getFilas() && i < 3; i++) {
            for (int j = 0; j < campo.getColumnas() && j < 3; j++) {
                campo.setValor(i, j, 10.0 + i * 2.5 + j * 1.8);
            }
        }
        
        menuPrincipal(campo);
        
    } else {
        std::cout << "Tipo invalido.\n";
    }
    
    std::cout << "\nSistema cerrado.\n";
    return 0;
}