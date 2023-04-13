import math 

#Сделал Дмитрий Московских

#Определяет четверть, в которую направлен вектор или четверть, в которой находится точка
def quarter(pos):            
    if pos[0] > 0 and pos[1] > 0: # x > 0 и y > 0 
        return 1 
    if pos[0] < 0 and pos[1] > 0: # x < 0 и y > 0
        return 2  
    if pos[0] < 0 and pos[1] < 0: # x < 0 и y < 0
        return 3 
    if pos[0] > 0 and pos[1] < 0: # x > 0 и y < 0
        return 4 
    if pos[0] == 0 and pos[1] == 0: 
        return 5               #Типа находится в точке (0,0)

class object(): 
    def __init__(self): 
        self.pos  = [0,0]  #Позиция      (Px, Py)
        self.vel  = [0,0]  #Скорость     (Vx, Vy)
        self.tpos = [0,0]  #Позиция цели (Px, Py)

    #Задать скорость в векторном значении
    def setVelocity(self, Vx, Vy): 
        self.vel[0] = Vx 
        self.vel[1] = Vy 
    
    #Задать позицию в векторном значении
    def setPosition(self, Px, Py):  
        self.pos[0] = Px 
        self.pos[1] = Py  
    
    def setTarget(self, Px, Py): 
        self.tpos[0] = Px 
        self.tpos[1] = Py  

    #Узнать расстояние до цели, определяется, как длинна вектора ( расстояние от точки RK до цели )
    def getTargetDistance(self): 
        return math.sqrt(self.tpos[0]**2 + self.tpos[1]**2)

    def getTarget(self): 
        return self.tpos 

    def getVelocity(self):
        return self.vel

    def getPosition(self):
        return self.pos

    #Тупо возвращает инфу об объекте. Пример: print( obj.info() ) -> [[x0,y0],[Vx,Vy]]
    def info(self):   
        return [self.pos, self.vel, self.tpos] 
#/////////////////////////////////////////////////////////////////////////////////////////////////////////////////// СВЕРХУ МОЁ 

def Work1(): 
    # ЧАСТЬ А 

    RK_2016 = object()
    RK_Target = object()  

    RK_Target.setPosition(6, 6)       #Заданы координаты цели (x0,y0) в нач. момент времени
    RK_Target.setVelocity(-4, -2)     #А также вектор (Vx,Vy) её скорости

    RK_2016.setPosition(0, 0)         #RK-2016 находится вначале координат

    #Учёт направления вектора скорости и позиции тел в про-ве
    if quarter(RK_Target.getPosition()) == quarter(RK_Target.getVelocity()) and quarter(RK_2016.getPosition()) != quarter(RK_Target.getPosition()): 
        print("Вектор скорости цели не направлен в сторону RK-2000") 
        return 
    #Это не очень идеальное решение, ибо есть случаи, когда две точки находятся в одной четверти, а вектор смотрит противоположно позиции RK_2016 

    print( RK_2016.info() )
    print( RK_Target.info() )

    RK_2016_Position = RK_2016.getPosition() 
    RK_Target_Position = RK_Target.getPosition()   
    RK_Target_Velocity = RK_Target.getVelocity()  

    # (x-x0)/Vx = (y - y0)/Vy -> Vx*y - Vy*X + Vy*x0 - Vx*y0 - неканон. ур-е прямой с параметрами цели. 
    # Далее мы используем формулу №1 (которую я закрепил в дис-дискордике) 

    # RK_Target_Position[0] - X 
    # RK_Target_Position[1] - Y 

    # RK_Target_Velocity[0] - Vx
    # RK_Target_Velocity[0] - Vy

    # Уж, разберётесь и сделайте замену в отчёте)) 

    distance_min = abs(RK_Target_Velocity[0]*RK_2016_Position[1] - RK_Target_Velocity[1]*RK_2016_Position[0] - RK_Target_Velocity[0]*RK_Target_Position[1] + RK_Target_Velocity[1]*RK_Target_Position[0]) / math.sqrt(RK_Target_Velocity[0]**2 + RK_Target_Velocity[1]**2) # Та самая формула №1
    print("Минимальное приближение цели к RK-2000: "+str(distance_min)) # str - функция, которая приводит любой тип данных к строковому виду, знак "+" - конкакетация(гуглите, что такое)

    #Ответ: 2.683... 
 
    #ЧАСТЬ B 

    RK_2016.setVelocity(10,10) # вектор скорости, мы можем найти его длинну и считать её максимальной скоростью V) 
    
    max_speed = math.sqrt(RK_2016.getVelocity()[0]**2 + RK_2016.getVelocity()[1]**2) # Вот так вот))

    #Какое-то время t и расстояние d, которое нам дано для того, чтобы ответить на вопрос из пункта b (необязательно тут должно стоять 5)
    time_to_target = 5   
    distance_to_target = 5     

    print(RK_Target_Position,RK_Target_Velocity); 

    RK_2016.setTarget(RK_Target_Position[0]+RK_Target_Velocity[0]*time_to_target, RK_Target_Position[1]+RK_Target_Velocity[1]*time_to_target)
    #Задаём координаты цели, которые она принимает при скорости V за время t
    
    print(RK_2016.info()[2])

    print("Расстояние до цели за t: " + str(RK_2016.getTargetDistance()))

    if time_to_target >= ((RK_2016.getTargetDistance() - distance_to_target) / max_speed ): # t = S/V, условие b выполняется, если времени было потрачено не больше, чем нам дано в переменной time_to_target 
        print("Да, может, за: " + str(((RK_2016.getTargetDistance() - distance_to_target) / max_speed ))) # у.е. - условных единиц (часы, минуты и т.д) 
        print("Скорость RK-2000: "+ str(max_speed)+" у.е.")  
        print("Расстояние до цели: " + str(RK_2016.getTargetDistance())+" у.е.")
    else: 
        print("Нет")




# Сделал Шемякин Савелий

#def Work2():

    #RK__2022 = object()
#
    #RK__2022.setPosition(0, 0) # Зададны координаты (х0; y0) в начальный момент времени.
#
    #Position__cel1 = (x1; y1) # Позиция цели в 
    #Position__cel2 = (x2; y2) # которую нужно попасть с начала координат то есть с RK__2022.
#
    #print ("Введите x1")
    #print ("Введите y1")
    #print ("Введите x2")
    #print ("Введите y2")
    #
#
    #Position__del1 = sqrt((x2 - x1)**2 + (y2 - y1)**2) # Высчитываем длину вектора. 
#
    #Position__del2 = Position__cel3 / 2 # Делим этот вектор пополам для того что бы найти длину этих обоих векторов.
#
    #if (Position__cel1 < Position__cel2)
    #{
    #  Position__del3 = sqrt((x1 - 0)**2 + (y1 - 0)**2) # Находим длину вектора от начала координат до блежайшей точки.
    #  print("Position__del3")
    #}
    #else (Position__cel1 > Position__cel2)
    #{
    #  Position__del3 = sqrt((x2 - 0)**2 + (y2 - 0)**2) # Находим длину вектора от начала координат до блежайшей точки.
    #  print("Position__del3")
    #}
#
    #Position__centr = Position__del3 + Position__del1 # Находим центер позиции в которую нужно попасть.
#
    #print ("Введите Position__sckorost")
    #A1 = arksin ((Position__centr * 9.81) / Position__sckorost) # Находим неизвестный угол sin.
    #A2 = A1 / 2 # теперь делим этот угол на 2 и получаем ответ.
#
    #print ("%i", A2)







def Init(): #сюда все функции Work пихать)))) 
    Work1()
    #Work2()





Init()  