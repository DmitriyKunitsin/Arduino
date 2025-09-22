#include <TimeManager.hpp>

void TimeManager::begin()
{
    _ntpClient.begin();
}
void TimeManager::update()
{
    _ntpClient.update();
}
String TimeManager::getFormattedTime()
{
    return _ntpClient.getFormattedTime();
}
String TimeManager::GetFormattedTimeForDisplay()
{
    unsigned long rawTime = _ntpClient.getEpochTime();
    unsigned long hours = (rawTime % 86400L) / 3600;
    String hoursStr = hours < 10 ? "0" + String(hours) : String(hours);

    unsigned long minutes = (rawTime % 3600) / 60;
    String minuteStr = minutes < 10 ? "0" + String(minutes) : String(minutes);
    return hoursStr + ":" + minuteStr;
}