#ifndef __CLOCKPROVIER_H__
#define __CLOCKPROVIER_H__

class ClockProvider{
    private:
        
    public:
        ClockProvider();
        void NtpSynchronize();
        unsigned long geTime();
        uint8_t getHours();
        uint8_t getMinutes();
        uint8_t getSeconds();
        int elapsedInMinutes(unsigned long start_time);

};
#endif // __CLOCKPROVIER_H__