class TimerAutoWork
{
private:
    int hour;
    int minute;
    int second;
    float ph;
    bool status;
    bool delete_;

public:
    TimerAutoWork()
    {
        hour = -1;
        minute = -1;
        second = -1;
        ph = -1;
        status = false;
        delete_ = true;
    }

    void setHour(int hourIn)
    {
        hour = hourIn;
    }

    void setMinute(int minuteIn)
    {
        minute = minuteIn;
    }

    void setSecond(int secondIn)
    {
        second = secondIn;
    }

    void setPH(float phIn)
    {
        ph = phIn;
    }

    void setStatus(bool statusIn)
    {
        status = statusIn;
    }

    void setDelete(bool deleteIn)
    {
        delete_ = deleteIn;
    }

    int getHour()
    {
        return hour;
    }

    int getMinute()
    {
        return minute;
    }

    int getSecond()
    {
        return second;
    }

    float getPH()
    {
        return ph;
    }

    bool getStatus()
    {
        return status;
    }

    bool getDelete()
    {
        return delete_;
    }

    String toString()
    {
        char buffer[50];

        sprintf(buffer, "%02d:%02d:%02d  status = %s   ph = %.1f   delete = %s", hour, minute, second, String(status), ph, String(delete_));

        String result = String(buffer);

        return result;
    }
};