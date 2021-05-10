import java.util.Arrays;

//classe da estrutura de dados (recurso) compartilhado entre as threads
class S {
    //recurso compartilhado
    public int[] array = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    public int index = 0;
    public int length = 10;

    public synchronized void inc() { 
        this.array[index]++;
        this.index++;
    }

    public synchronized int[] get() { 
        return this.array; 
    }
}

//classe que estende Thread e implementa a tarefa de cada thread do programa 
class T extends Thread {
   //identificador da thread
   private int id;
   //objeto compartilhado com outras threads
   S s;
  
   //construtor
   public T(int tid, S s) { 
      this.id = tid; 
      this.s = s;
   }

   //metodo main da thread
   public void run() {
        System.out.println("Thread " + this.id + " iniciou!");
        
        while(this.s.index < this.s.length){
            this.s.inc();  
        }

        System.out.println("Thread " + this.id + " terminou!"); 
   }
}

//classe da aplicacao
class Lab6 {
   static final int N = 3;

   public static void main (String[] args) {
      //reserva espaço para um vetor de threads
      Thread[] threads = new Thread[N];

      //cria uma instancia do recurso compartilhado entre as threads
      S s = new S();

      //cria as threads da aplicacao
      for (int i=0; i<threads.length; i++) {
         threads[i] = new T(i, s);
      }

      //inicia as threads
      for (int i=0; i<threads.length; i++) {
         threads[i].start();
      }

      //espera pelo termino de todas as threads
      for (int i=0; i<threads.length; i++) {
         try { threads[i].join(); } catch (InterruptedException e) { return; }
      }

      System.out.println("Vetor resultante: "); 
      System.out.println(Arrays.toString(s.get())); 
   }
}
