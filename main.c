#include <stdio.h>
#include <string.h>
#include "funciones.h"


int main (int argc, char *argv[]) {

    struct Producto productos[5];
    int opc;
    
    do {
        opc=menu();
        
        switch (opc)
        {
        case 1:
            inicializarProductos(productos);
            printf("Productos inicializados correctamente.\n");
            break;
        case 2:
            imprimirProductos(productos);
            break;
        case 3:
            realizarVenta(productos);
            break;
        case 4:
            listarVentas();
            break;
        case 5:
            int cedula;
            printf("Ingrese la cedula del cliente: ");
            scanf("%d", &cedula);
            buscarVenta(cedula);
            break;
        case 6:
            resumenProductosVendidos();
            break;
        case 7:
            agregarInventario();
            break;

        default:
            break;
        }

       
    } while(opc!=8);
    

    return 0;
}