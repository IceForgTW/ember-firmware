/* 
 * File:   PrintEngine.h
 * Author: Richard Greene
 *
 * Created on April 8, 2014, 2:18 PM
 */

#ifndef PRINTENGINE_H
#define	PRINTENGINE_H

#include <map>

#include <PrinterStatus.h>
#include <Event.h>
#include <Motor.h>
#include <FrontPanel.h>
#include <Commands.h>
#include <Projector.h>
#include <Error.h>

#define SEPARATION_TIME_SEC (6.5)  // time required to separate from each layer
#define DEFAULT_MOTOR_TIMEOUT_SEC (30) // default timeout for motor command completion
#define LONGER_MOTOR_TIMEOUT_SEC (60) // timeout for longer motor command completion
#define LONGEST_MOTOR_TIMEOUT_SEC (120) // timeout for longest motor command completion

class PrinterStateMachine;

/// The class that controls the printing process
class PrintEngine : public ICallback, public ICommandTarget
{
public: 
    PrintEngine(bool haveHardware);
    ~PrintEngine();
    void SendStatus(PrintEngineState state, StateChange change = NoChange, 
                    UISubState substate = NoUISubState);
    void SetNumLayers(int numLayers);
    int NextLayer();
    int GetCurrentLayer() { return _printerStatus._currentLayer; }
    int SetCurrentLayer(int layer) { _printerStatus._currentLayer = layer; }
    bool NoMoreLayers();
    void SetEstimatedPrintTime(bool set);
    void DecreaseEstimatedPrintTime(int amount);
    int GetExposureTimerFD();
    int GetMotorTimeoutTimerFD();
    void StartExposureTimer(double seconds);
    void ClearExposureTimer();
    void StartMotorTimeoutTimer(int seconds);
    void ClearMotorTimeoutTimer();
    int GetStatusUpdateFD();
    void Initialize();
    void SendMotorCommand(unsigned char command);
    void SendMotorCommand(const unsigned char* commandString);
    void Begin();
    void CancelPrint();
    double GetExposureTimeSec();
    int GetRemainingExposureTimeSec();
    void StopMotor();
    bool DoorIsOpen();
    I2C_Device* GetMotorBoard() { return _pMotor; }
    void ShowImage();
    void ShowBlack();
    bool TryStartPrint();
    bool SendSettings();
    void HandleError(ErrorCode code, bool fatal = false, 
                     const char* str = NULL, int value = INT_MAX);
    void ClearError();
    void ProcessData();

#ifdef DEBUG
    // for testing only 
    PrinterStateMachine* GetStateMachine() { return _pPrinterStateMachine; }
#endif
    
private:
    int _exposureTimerFD;    
    int _motorTimeoutTimerFD;
    int _statusReadFD;
    int _statusWriteFd;
    PrinterStatus _printerStatus;
    PrinterStateMachine* _pPrinterStateMachine;
    Motor* _pMotor;
    long _printStartedTimeMs;
    int _initialEstimatedPrintTime;
    Projector _projector;
    bool _awaitingMotorSettingAck;
    std::map<const char*, const char*> _motorSettings;
    bool _haveHardware;

    PrintEngine(); // need to specify if we have hardware in c'tor

    virtual void Callback(EventType eventType, void* data);
    virtual void Handle(Command command);
    void MotorCallback(unsigned char *status);
    void DoorCallback(char* data);
    bool IsFirstLayer();
    bool IsBurnInLayer();
}; 

#endif	/* PRINTENGINE_H */

