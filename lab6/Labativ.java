import java.util.Random;

class Vetor {
    //Atributos Vetor
    private int [] vetor;
    private int tamanho;
    private int pares;
    private Random random;


    public Vetor(int tamanho) { //Construtor da classe
        this.tamanho = tamanho;
        this.vetor = new int [tamanho];
        this.random = new Random();
        for(int i = 0; i < tamanho; i++) {
            this.vetor[i] = random.nextInt(1000);
        }
        this.pares = 0;
    }
    //Métodos
    public int getTamanho() {
        return this.tamanho;
    }


    public int getElemento(int posicao) {
        return this.vetor[posicao];
    }


    public void imprimeVetor() {
        String saida = "[";
        for (int i = 0; i < this.vetor.length; i++) {
            saida = saida + this.vetor[i];
            if(i != this.vetor.length - 1) {
                saida = saida + ", ";
            }
        }

        saida = saida + "]";
        System.out.println(saida);
    }


    public int getPares() {
        return pares;
    }

    public void setPares() {
        this.pares++;
    }

    public void imprimirQuantidadeImpares(){
        System.out.println("Impares: " + (getTamanho() - getPares()));
    }
}
class Contador implements Runnable {


    //Atributos da classe Somador
    Vetor a; //Vetor que sera somado
    int id; //Identificador da thread
    int nthreads; //Armazena o número de threads sendo utilizada para saber quais elementos esta thread irá calcular


    public Contador(Vetor a, int id, int nthreads) { //Construtor da thread
        this.a = a;
        this.id = id;
        this.nthreads = nthreads;
    }
    @Override
    public void run() {
        for(int i = this.id; i < this.a.getTamanho(); i += this.nthreads) {
            if(this.a.getElemento(i) % 2 == 0) {
                this.a.setPares();
            }
        }
    }
}
public class Labativ{
    public static void main(String [] args) {
        int tamanho=100; //Variável que armazenará o tamanho dos vetores
        Vetor a = new Vetor(tamanho);
        int nthreads = 10; //Variável que armazenará o número de threads


        //Imprime o vetor inicial
        System.out.print("Vetor inicial: "); a.imprimeVetor();


        Thread [] threads = new Thread[nthreads]; //Vetor de Threads instanciado


        //Cria as threads da aplicação
        for(int i = 0; i < nthreads; i++) {
            threads[i] = new Thread(new Contador(a, i, nthreads));
        }


        //Inicia as threads
        for(int i = 0; i < nthreads; i++) {
            threads[i].start();
        }

        //Espera o término de todas as threads
        for(int i = 0; i < nthreads; i++) {
            try { threads[i].join(); } catch (InterruptedException e) { 
                return; 
            }
        }
        //Imprime o vetor resultante e termina a execução da main
        System.out.println("Pares: " + a.getPares());
        a.imprimirQuantidadeImpares();
    }
}