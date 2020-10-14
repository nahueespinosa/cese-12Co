//Me permite usar la sintaxis unificada con C (establecer comandos del prepro, por ejemplo)
	.syntax unified



//.text Indica que lo que sigue va en flash
	.text


//Ac谩 va la definici贸n de la funci贸n. La etiqueta debe coincidir con el nombre en C
	.global ajustarBrillo_simd

//Esto indica que a continuaci贸n se utiliza c贸digo Thumb
	.thumb_func

//Prototipo de la funci贸n

//void ajustarBrillo_asm (uint8_t ** imagen, uint32_t ancho, uint32_t alto, int8_t brillo);

#define ANCHO_IMAGEN 64
#define ALTO_IMAGEN  48

// Argumentos
#define imagen      r0
#define ancho       r1
#define alto        r2
#define brillo      r3

// "Variables locales"
#define indice      r4
#define longitud    r5
#define valor       r6


ajustarBrillo_simd:
    push   {r4-r6,lr}

    mul    longitud, ancho, alto
    lsr    longitud, #2             // Divido por 4 la longitud ya que voy a cargar de a 4 bytes
    ldr    indice, =0

    cmp    brillo, #0
    blt    negativo

    bfi    brillo, brillo, #8, #8
    bfi    brillo, brillo, #16, #16

// Si el brillo es positivo se ejecuta esta secci髇 del c骴igo
loop_positivo:
    ldr    valor, [imagen, indice, LSL 2]

    uqadd8 valor, valor, brillo     // Suma sin signo con saturaci髇 de 8 bits

    str    valor, [imagen, indice, LSL 2]

    add    indice,1
    cmp    longitud, indice
    bne    loop_positivo

    b      exit

// Si el brillo es negativo se ejecuta esta secci髇 del c骴igo
negativo:
    neg    brillo, brillo           // Me quedo con el valor absoluto del brillo
    bfi    brillo, brillo, #8, #8
    bfi    brillo, brillo, #16, #16

loop_negativo:
    ldr    valor, [imagen, indice, LSL 2]

    uqsub8 valor, valor, brillo     // Resta sin signo con saturaci髇 de 8 bits

    str    valor, [imagen, indice, LSL 2]

    add    indice,1
    cmp    longitud, indice
    bne    loop_negativo

exit:
    pop    {r4-r6,pc}


