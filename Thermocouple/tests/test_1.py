from testinitialization import *

# Tests temperature tolerance in Celsius
tolerance = 0.5

def test_innercases():
    assert isclose(tc.TC_Volts2Temp(0, 0), 0, tol=tolerance)
    assert isclose(tc.TC_Volts2Temp(39.314, 0), 950, tol=tolerance)
    assert isclose(tc.TC_Volts2Temp(40.101, 0), 970, tol=tolerance)
    assert isclose(tc.TC_Volts2Temp(51, 0), 1260, tol=tolerance)
    assert isclose(tc.TC_Volts2Temp(7.34, 0), 180, tol=tolerance)
    assert isclose(tc.TC_Volts2Temp(42.053, 0), 1020, tol=tolerance)


def test_cornercases():
    assert isclose(tc.TC_Volts2Temp(-10, 0), -270, tol=tolerance)
    assert isclose(tc.TC_Volts2Temp(-20, 0), -270, tol=tolerance)
    assert isclose(tc.TC_Volts2Temp(-6.458, 0), -270, tol=tolerance)
    assert isclose(tc.TC_Volts2Temp(100, 0), 1370, tol=tolerance)
    assert isclose(tc.TC_Volts2Temp(55, 0), 1370, tol=tolerance)
    assert isclose(tc.TC_Volts2Temp(54.819, 0), 1370, tol=tolerance)

def test_temperature():
    assert isclose(tc.TC_Volts2Temp(0, 100), 100, tol=tolerance)
    assert isclose(tc.TC_Volts2Temp(7.34, 100), 280, tol=tolerance)
    assert isclose(tc.TC_Volts2Temp(42.053, 1000), 2020, tol=tolerance)

    