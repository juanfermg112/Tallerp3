struct Cliente
{
    char nombre[30];
    int cedula;
};


struct Producto
{
    char nombre[20];
    float precio;
    int stock;
};

struct venta
{
    struct Cliente cliente;
    int numProductos;
    struct Producto productos[5];
    int cantidades[5]; // Array para guardar las cantidades vendidas de cada producto
    float totalVenta;
};

int menu();
void leercadena(char *cadena, int num);
void inicializarProductos(struct Producto productos[5]);
void imprimirProductos(struct Producto productos[5]);
void guardarProductos(struct Producto *productos);
int leerProductos(struct Producto *productos);
void realizarVenta(struct Producto productos[5]);
void listarVentas();
void buscarVenta(int cedula);
void resumenProductosVendidos();
void agregarInventario();