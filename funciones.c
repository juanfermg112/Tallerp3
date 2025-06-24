#include <stdio.h>
#include <string.h>
#include "funciones.h"

int menu(){
    int opc;
    printf("Sistema de Ventas\n");
    printf("-----------------\n");
    printf("Seleccione una opcion> \n");
    printf("1. Inicializar productos\n");
    printf("2. Informacion de productos\n");
    printf("3. Realizar venta\n");
    printf("4. Listar venta\n");
    printf("5. Buscar venta\n");
    printf("6. Resumen de productos vendidos\n");
    printf("7. Agregar inventario\n");
    printf("8. Salir....\n");
    printf(">> ");
    fflush(stdin);
    scanf("%d", &opc);
    return opc;
}


void leercadena(char *cadena, int num){
    fflush(stdin);
    fgets(cadena,num,stdin);
    int len = strlen(cadena) -1;
    cadena[len] = '\0';
}

void inicializarProductos(struct Producto productos[5]){
    for (int i = 0; i < 5; i++) {
        printf("Producto %d:\n", i + 1);
        printf("Ingrese el nombre del producto: ");
        leercadena(productos[i].nombre, 20);
        printf("Ingrese el precio del producto: ");
        scanf("%f", &productos[i].precio);
        printf("Ingrese el stock del producto: ");
        scanf("%d", &productos[i].stock);
        printf("\n");
    }
    guardarProductos(productos);
}

void imprimirProductos(struct Producto productos[5]){
    if (leerProductos(productos) == 1)
    {
        printf("Productos:\n");
        printf("#\t\tNombre\t\t\t\tPrecio\t\tStock\n");
        for (int i = 0; i < 5; i++)
        {
            printf("%d,\t\t%s\t\t\t%.2f\t\t%d\n", i+1, productos[i].nombre,
                                            productos[i].precio, productos[i].stock);
        }
    }
}

void guardarProductos(struct Producto *productos){
    FILE *f;
    f=fopen("productos.dat", "wb+");
    fwrite(productos, sizeof(struct Producto),5,f);
    fclose(f);
} //Guarda 5 produtos en una sola escritura


int leerProductos(struct Producto *productos){
    FILE *f;
    f=fopen("productos.dat", "rb+");
    if (f==NULL)
    {
        printf("No existe el archivo\n");
        return 0;
    }
    fread(productos, sizeof(struct Producto), 5,f);
    fclose(f);
    return 1;
}  

void realizarVenta(struct Producto productos[5]){
    struct venta venta;
    int opc, i;
    float total = 0;

    if (leerProductos(productos) != 1) {
        printf("Error: No se pudieron cargar los productos.\n");
        return;
    }

    printf("Productos disponibles:\n");
    imprimirProductos(productos);

    printf("Ingrese el nombre del cliente: ");
    leercadena(venta.cliente.nombre, 30);
    printf("Ingrese la cedula del cliente: ");
    scanf("%d", &venta.cliente.cedula);

    venta.numProductos = 0;
    total = 0;

    do {
        printf("Seleccione un producto por su numero (0 para finalizar): ");
        scanf("%d", &opc);

        if (opc == 0) break;

        if (opc < 1 || opc > 5) {
            printf("Opción inválida. Intente nuevamente.\n");
            continue;
        }

        // Verificar stock disponible
        if (productos[opc - 1].stock <= 0) {
            printf("Producto sin stock disponible.\n");
            continue;
        }

        // Preguntar cantidad deseada
        int cantidad;
        printf("Stock disponible: %d\n", productos[opc - 1].stock);
        printf("Cuantas unidades desea llevar? ");
        scanf("%d", &cantidad);

        if (cantidad <= 0) {
            printf("Cantidad invalida.\n");
            continue;
        }

        if (cantidad > productos[opc - 1].stock) {
            printf("No hay suficiente stock. Stock disponible: %d\n", productos[opc - 1].stock);
            continue;
        }

        // Se agrega el producto seleccionado a la venta
        venta.productos[venta.numProductos] = productos[opc - 1];
        venta.cantidades[venta.numProductos] = cantidad; // Guardar la cantidad comprada en el array específico
        // Se suma el precio del producto multiplicado por la cantidad al total de la venta
        total += productos[opc - 1].precio * cantidad;
        // Actualizar stock del producto
        productos[opc - 1].stock -= cantidad;
        // Se incrementa el contador de productos en la venta
        venta.numProductos++;

        if (venta.numProductos >= 5) {
            printf("Se alcanzo el limite de productos por venta.\n");
            break;
        }

    } while (opc != 0);

    venta.totalVenta = total;

    printf("Resumen de la venta:\n");
    printf("Cliente: %s\n", venta.cliente.nombre);
    printf("Cedula: %d\n", venta.cliente.cedula);
    printf("Productos comprados:\n");
    for (i = 0; i < venta.numProductos; i++) {
        printf("- %s: %.2f x %d = %.2f\n", venta.productos[i].nombre, 
               venta.productos[i].precio, venta.cantidades[i], 
               venta.productos[i].precio * venta.cantidades[i]);
    }
    printf("Total: %.2f\n", venta.totalVenta);

    // Guardar productos actualizados con el nuevo stock
    guardarProductos(productos);

    FILE *f = fopen("ventas.dat", "ab");
    if (f == NULL) {
        printf("Error al guardar la venta.\n");
        return;
    }
    fwrite(&venta, sizeof(struct venta), 1, f);
    fclose(f);

    printf("Venta registrada correctamente.\n");
}

void listarVentas() {
    struct venta venta;
    FILE *f = fopen("ventas.dat", "rb");
    if (f == NULL) {
        printf("No hay ventas registradas.\n");
        return;
    }

    printf("Ventas registradas:\n");
    while (fread(&venta, sizeof(struct venta), 1, f) == 1) {
        printf("Cliente: %s, Cedula: %d, Total: %.2f\n", 
               venta.cliente.nombre, venta.cliente.cedula, venta.totalVenta);
    }
    fclose(f);
}

void buscarVenta(int cedula) {
    struct venta venta;
    FILE *f = fopen("ventas.dat", "rb");
    if (f == NULL) {
        printf("No hay ventas registradas.\n");
        return;
    }

    int encontrado = 0;
    while (fread(&venta, sizeof(struct venta), 1, f) == 1) {
        if (venta.cliente.cedula == cedula) {
            printf("Venta encontrada:\n");
            printf("Cliente: %s, Cedula: %d, Total: %.2f\n", 
                   venta.cliente.nombre, venta.cliente.cedula, venta.totalVenta);
            encontrado = 1;
            break;
        }
    }
    fclose(f);

    if (!encontrado) {
        printf("No se encontro una venta para la cedula %d.\n", cedula);
    }
}

void resumenProductosVendidos() {
    struct venta venta;
    struct Producto productos[5];
    char nombresProductos[5][20];
    int totalVendidos[5] = {0}; // Array para acumular cantidades vendidas de cada producto
    float ingresosPorProducto[5] = {0}; // Array para acumular ingresos por producto
    int productosEncontrados = 0;
    
    FILE *f = fopen("ventas.dat", "rb");
    if (f == NULL) {
        printf("No hay ventas registradas.\n");
        return;
    }

    // Leer todas las ventas y acumular por producto
    while (fread(&venta, sizeof(struct venta), 1, f) == 1) {
        for (int i = 0; i < venta.numProductos; i++) {
            // Buscar si ya tenemos este producto en nuestro registro
            int productoIndex = -1;
            for (int j = 0; j < productosEncontrados; j++) {
                if (strcmp(venta.productos[i].nombre, nombresProductos[j]) == 0) {
                    productoIndex = j;
                    break;
                }
            }
            
            // Si es un producto nuevo, agregarlo
            if (productoIndex == -1 && productosEncontrados < 5) {
                strcpy(nombresProductos[productosEncontrados], venta.productos[i].nombre);
                productoIndex = productosEncontrados;
                productosEncontrados++;
            }
            
            // Acumular cantidades e ingresos
            if (productoIndex != -1) {
                totalVendidos[productoIndex] += venta.cantidades[i]; // Usar el campo cantidades específico
                ingresosPorProducto[productoIndex] += venta.productos[i].precio * venta.cantidades[i];
            }
        }
    }
    fclose(f);

    printf("=== RESUMEN DE PRODUCTOS VENDIDOS ===\n");
    printf("Producto\t\tCantidad Vendida\tIngresos Totales\n");
    printf("-------------------------------------------------------\n");
    
    if (productosEncontrados == 0) {
        printf("No se han vendido productos aún.\n");
    } else {
        for (int i = 0; i < productosEncontrados; i++) {
            printf("%s\t\t\t%d\t\t\t%.2f\n", nombresProductos[i], totalVendidos[i], ingresosPorProducto[i]);
        }
    }
}

void agregarInventario() {
    struct Producto productos[5];
    int opcion, cantidadAgregar;
    
    if (leerProductos(productos) != 1) {
        printf("Error: No se pudieron cargar los productos. Primero debe inicializar productos.\n");
        return;
    }
    
    printf("=== AGREGAR INVENTARIO ===\n");
    printf("Productos actuales:\n");
    printf("#\t\tNombre\t\t\t\tPrecio\t\tStock Actual\n");
    for (int i = 0; i < 5; i++) {
        printf("%d,\t\t%s\t\t\t%.2f\t\t%d\n", i+1, productos[i].nombre,
                                        productos[i].precio, productos[i].stock);
    }
    
    printf("\nSeleccione el producto al que desea agregar stock (1-5, 0 para cancelar): ");
    scanf("%d", &opcion);
    
    if (opcion == 0) {
        printf("Operacion cancelada.\n");
        return;
    }
    
    if (opcion < 1 || opcion > 5) {
        printf("Opcion invalida.\n");
        return;
    }
    
    printf("Producto seleccionado: %s\n", productos[opcion - 1].nombre);
    printf("Stock actual: %d\n", productos[opcion - 1].stock);
    printf("¿Cuantas unidades desea agregar al inventario? ");
    scanf("%d", &cantidadAgregar);
    
    if (cantidadAgregar <= 0) {
        printf("Cantidad invalida. Debe ser mayor a 0.\n");
        return;
    }
    
    // Agregar la cantidad al stock existente
    productos[opcion - 1].stock += cantidadAgregar;
    
    // Guardar los productos actualizados
    guardarProductos(productos);
    
    printf("Inventario actualizado exitosamente.\n");
    printf("Producto: %s\n", productos[opcion - 1].nombre);
    printf("Stock anterior: %d\n", productos[opcion - 1].stock - cantidadAgregar);
    printf("Stock agregado: %d\n", cantidadAgregar);
    printf("Stock nuevo: %d\n", productos[opcion - 1].stock);
}
