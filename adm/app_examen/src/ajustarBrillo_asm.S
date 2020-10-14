//Me permite usar la sintaxis unificada con C (establecer comandos del prepro, por ejemplo)
	.syntax unified



//.text Indica que lo que sigue va en flash
	.text


//Acá va la definición de la función. La etiqueta debe coincidir con el nombre en C
	.global ajustarBrillo_asm

//Esto indica que a continuación se utiliza código Thumb
	.thumb_func

//Prototipo de la función

//void ajustarBrillo_asm (uint8_t ** imagen, uint32_t ancho, uint32_t alto, int8_t brillo);

#define ANCHO_IMAGEN 64
#define ALTO_IMAGEN  48

#define imagen      r0
#define ancho       r1
#define alto        r2
#define brillo      r3
#define indice      r4
#define longitud    r5
#define valor       r6


ajustarBrillo_asm:
	push   {r4-r6,lr}

	mul    longitud, ancho, alto
	ldr    indice, =0

loop:
    ldrb   valor, [imagen, indice]

    add    valor, brillo
    usat   valor, 8, valor

    strb   valor, [imagen, indice]

    add    indice,1
    cmp    longitud, indice
    bne    loop

	pop    {r4-r6,pc}
