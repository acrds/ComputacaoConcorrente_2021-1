class Buffer{

    public int tamanhoBuffer=10;
    private int count=0;// quantidade de elementos no buffer
    private int in=0;// posicão do buffer de insercao de threads produtoras
    private int out=0;// posição do buffer de remocao de threads consumidoras 
    int[] buffer = new int[tamanhoBuffer];

   
    public synchronized void ProdutorInsere(int id,int elemento){
        try{
            while(count==tamanhoBuffer){
                System.out.println("buffer.insercaoBloqueada("+id+")");
                wait();
            }
            buffer[in]=elemento;
            in=(in+1)%tamanhoBuffer;
            count++;
            System.out.println("A thread ("+id+") de produtor inseriu "+id);
            imprimeBuffer();
            notifyAll();
        } catch(InterruptedException e){ }
    }

    public synchronized void imprimeBuffer(){
        System.out.println("----buffer----");
        for(int i=0; i<this.tamanhoBuffer; i++){
            System.out.print(buffer[i]+" ");
        }
        System.out.println("");
    }
 

    public synchronized void ConsumidorRemove(int id){
        int elemento;
        try{
            while(count==0){
                System.out.println("buffer.remocaoBloqueada("+id+")");
                wait();
            }
            elemento = buffer[out];
            buffer[out] = 0;
            out = (out + 1) % tamanhoBuffer;
            count--;
            System.out.println("A thread ("+id+") de consumidor removeu "+elemento);
            imprimeBuffer();
            notifyAll();
        } catch(InterruptedException e){ }
    }
}

// Consumidor
class Consumidor extends Thread{
    int id; // identificador da thread
    int delay; // atraso bobo
    Buffer monitor;  // objeto monitor para coordenar a lógica de execução das threads

    // Construtor
    Consumidor(int id, int delay, Buffer monitor){
        this.id = id;
        this.delay = delay;
        this.monitor = monitor;
    }

    // Método executado pela thread
    public void run(){
        try{
            while(true){
                this.monitor.ConsumidorRemove(id);
                sleep(this.delay);
            }
        } catch(InterruptedException e){ }
    }
}
// ----------------------------------------------------------------

//Produtor
class Produtor extends Thread{
    int id; // identificador da thread
    int delay; // atraso bobo
    Buffer monitor; // objeto monitor para coordenar a lógica de execução das threads

    Produtor(int id, int delay, Buffer monitor){
        this.id = id;
        this.delay = delay;
        this.monitor = monitor;
    }

    // Método executado pela thread
    public void run(){
        try{
            while(true){
                this.monitor.ProdutorInsere(id, id);
                sleep(this.delay);
            }
        } catch(InterruptedException e){ }
    }
}


// Classe principal
public class ProdutorConsumidor {
    static final int C = 3; 
    static final int P = 3; 
    
    public static void main(String[] args){
        Buffer monitor = new Buffer(); // monitor
        Consumidor[] c = new Consumidor[C]; // threads consumidoras
        Produtor[] p = new Produtor[P]; // threads produtoras
    
        //log de saída
        System.out.println("import verificaBuffer");
        System.out.println("buffer = verificaBuffer.Buffer()");

        for(int i=0; i<C; i++){
            c[i] = new Consumidor(i+1, (i+1)*500, monitor);
            c[i].start();
        }
        for(int i=0; i<P; i++){
            p[i] = new Produtor(i+1, (i+1)*500, monitor);
            p[i].start();
        }

    }   
}