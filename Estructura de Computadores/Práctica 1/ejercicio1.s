


	Compute_Integral:
    	#######################################################
    	#Recibe: a0 = a, a1 = b, a2 = p, a3 = q, a4 = r, a5 = N
        #Devuelve fa0 = resultado integral
        #######################################################
        
        #Apilar registros s y ra
        addi sp sp -20
        sw ra 16(sp)
        fsw fs0 12(sp)
        sw s0 8(sp)
        fsw fs1 4(sp)
        fsw fs2 0(sp)
        
        
        #Convertir a, b, N a float
        fcvt.s.w ft0 a0 #ft0 = a
        fcvt.s.w ft1 a1 #ft1 = b
        fcvt.s.w ft2 a5 #ft2 = N
        
        #Calculo h
        fsub.s ft4 ft1 ft0 #b-a
        fdiv.s fs0 ft4 ft2 # dividir entre N
        
        #Iniciar en n = 0
        li s0 0
        fcvt.s.w fs1 t0
        
        #Iniciar la suma = 0
        fmv.w.x fs2 zero
        
        loop_integral:
        	bge s0 a5 end_loop #n>=N saltar a end_loop
            
            #Calcular x
            fmul.s ft3 fs1 fs0 #ft3 = fs1*fs0 -> n*h
            fadd.s ft5 ft3 ft0 #ft5 = ft3 + ft0 = n*h + a
            
            #Apilar los registros
            addi sp sp -24
            sw a5 20(sp)
            fsw ft3 16(sp)
            fsw ft0 12(sp)
            sw a2 8(sp)
            sw a3 4(sp)
            sw a4 0(sp)
            
            #Mover los registros para posteriormente llamar a f
            mv a0 a2 #p
            mv a1 a3 #q
            mv a2 a4 #r
            fmv.s fa0 ft5 #x
            
            #Saltar a f
            jal ra f
            
            #Desapilar los registros
            lw a4 0(sp)
            lw a3 4(sp)
            lw a2 8(sp)
            flw ft0 12(sp)
            flw ft3 16(sp)
            lw a5 20(sp)
            addi sp sp 24
            
            #Actualizar n
            addi s0 s0 1 #n += 1
            fcvt.s.w fs1 s0 #Pasar n a float
            
            #Actualizar la suma
            fadd.s fs2 fs2 fa0
            #Saltar al bucle
            j loop_integral
        
        end_loop:
        	#Multiplicar suma*h = resultado final
        	fmul.s fa0 fs2 fs0
            
            #Desapilar registros s y ra
            flw fs2 0(sp)
            flw fs1 4(sp)
            lw s0 8(sp)
            flw fs0 12(sp)
            lw ra 16(sp)
            addi sp sp 20
            
            jr ra
            
        
    

	f:
        #######################################################
    	#Recibe: a0 = p, a1 = q, a2 = r, fa0 = x
        #Devuelve fa0 = Resultado f(x)
        #######################################################
      
      #Apilar los registros fs
      addi sp sp -20
      sw ra 16(sp)
      fsw fs2 12(sp)
      fsw fs3 8(sp)
      fsw fs4 4(sp)
      fsw fs5 0(sp)
      
      ##Apilar los registros que se le pasan a la funcion
      addi sp sp -16
      sw a2 12(sp)
      sw a1 8(sp)
      fsw fa0 4(sp)
      sw a0 0(sp)

	  #Funcion pow
      #Recibe el exponente en a0 y x en fa0
      li a0 2 #Cargar el exponente en a0
      jal ra pow

      #Calcular p*x**2
      lw a0 0(sp) #Recuperamos a0 de la pila
      fcvt.s.w fs2 a0 #fs2 = p(float)
      fmul.s fs2 fs2 fa0 #fs2 = fa0(x^2)*fs2(p)

      #Calcular q*x
      flw ft1 4(sp) #Recuperar x
      lw a1 8(sp) #Recuperar q
      fcvt.s.w fs3 a1 #fs3 = q(float)
      fmul.s fs3 fs3 ft1 #fs3 = ft1(x)*fs3(q)

      #r
      lw a2 12(sp) #Recuperar r
      fcvt.s.w fs4 a2 #fs4 = r(float)
      addi sp sp 16
      
      #Sumar todo
      fadd.s fs5 fs3 fs2 #fs5 = p*x^2 + q*x
      fadd.s fs5 fs5 fs4 #fs5 = fs5 + r
      fmv.s fa0 fs5 #fa0 = fs5
      
      #Desapilar los registros
      flw fs5 0(sp)
      flw fs4 4(sp)
      flw fs3 8(sp)
      flw fs2 12(sp)
      lw ra 16(sp)
      addi sp sp 20
      
      jr ra