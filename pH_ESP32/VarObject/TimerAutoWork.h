class TimerAutoWork
{
private:
    int hour;
    int minute;
    float ph;
    bool status;
    bool delete_;

public:
    TimerAutoWork()
    {
        hour = -1;
        minute = -1;
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

        sprintf(buffer, "%02d:%02d:00  status = %s   ph = %.1f   delete = %s", hour, minute, String(status), ph, String(delete_));

        String result = String(buffer);

        return result;
    }
};