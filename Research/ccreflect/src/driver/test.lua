COROUTINES = {}

dt = 0

function LuaSetDT( t )
  dt = t
end

-- Cause a script to wait a certain amount of seconds
-- before resuming again.
function WaitSeconds( seconds )
  local t = 0
  print( "Waiting for about " .. seconds .. " seconds." )
  coroutine.yield( )
  while( true ) do
    t = t + dt
    if t > seconds then
      return
    else
      coroutine.yield( )
    end
  end
end

function AddCoroutine( id, co )
  COROUTINES[id] = co
  print( "Added coroutine." )
end

function RemoveCoroutine( id )
  COROUTINES[id] = nil
  print( "Removed coroutine." )
end

-- This acts as a script routine for
function TestUpdateRoutine( object )
  print( "Loaded coroutine: TestUpdateRoutine( knight )" )
  
  while( true ) do
    -- Pauses the script's execution until resume is called
    -- again. Resume is called within an object's update method.
    coroutine.yield( )
    
    print( "ObjectPrintInt being called..." )
    object:ObjectPrintInt( 1337 )
    
    -- Pause script, similar to coroutine.yield, except will not
    -- resume script execution until a time elapse
    WaitSeconds( 1 )
    
    -- Call a function on an object
    object:PrintName( )
    
    WaitSeconds( 2.5 )
    object:PrintALotOfStuff( 1, 2, 3 )
    
    WaitSeconds( 3 )
  end
end

print( "test.lua has been opened" )
