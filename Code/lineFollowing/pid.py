
class PID:
    #kp = ki = kd  = 0
    #target=0
   # erro = lastError = beforeError = 0

    def __init__(self,p=0,i=0,d=0):
        self.kp = float(p)
        self.ki = float(i)
        self.kd = float(d)
        #self.target=float(t)

    def get_pid(self,present):
        self.erro=self.target-present
        output=self.kp*(self.erro-self.lastError)+self.ki*(self.erro)+self.kd*(self.erro-2*self.lastError+self.beforeError)
        self.beforeError=self.lastError
        self.lastError=self.erro
        return get_pid


