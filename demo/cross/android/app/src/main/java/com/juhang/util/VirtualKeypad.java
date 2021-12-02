package com.juhang.util;

import android.app.Activity;
import android.view.KeyEvent;
import android.view.LayoutInflater;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;

import com.erz.joysticklibrary.JoyStick;

import org.libsdl.app.SDLActivity;

import static android.view.KeyEvent.ACTION_UP;
import static com.erz.joysticklibrary.JoyStick.DIRECTION_CENTER;
import static com.erz.joysticklibrary.JoyStick.DIRECTION_DOWN;
import static com.erz.joysticklibrary.JoyStick.DIRECTION_LEFT;
import static com.erz.joysticklibrary.JoyStick.DIRECTION_RIGHT;
import static com.erz.joysticklibrary.JoyStick.DIRECTION_UP;
import static com.juhang.green.R.*;
import static com.juhang.green.R.id.*;

public class VirtualKeypad  implements JoyStick.JoyStickListener  {

    static int KEY_LEFT = 0;
    static int KEY_RIGHT = 1;
    static int KEY_UP = 2;
    static int KEY_DOWN = 3;
    static int KEY_END = 4;
    int userDirection = DIRECTION_CENTER;

    int keyStates[];
    public static View m_inputView = null;

    Button aButton;
    Button bButton;
    Button cButton;

    Activity m_context;
    ViewGroup m_viewGroup = null;

    public ViewGroup GetView()
    {
        return m_viewGroup;
    }

    public VirtualKeypad(Activity context, ViewGroup viewGroup)
    {
        m_context = context;
        m_viewGroup = viewGroup;

        m_inputView = LayoutInflater.from(context).inflate(layout.layout, m_viewGroup, false);

        m_viewGroup.addView(m_inputView);

        keyStates = new int[4];

        for(int i = 0; i < KEY_END; i++)
        {
            keyStates[i] = 0;
        }

        JoyStick joystick = (JoyStick) m_inputView.findViewById(joy1);
        joystick.setType(JoyStick.TYPE_4_AXIS);
        joystick.setPadBackground(drawable.joystick_outer);
        joystick.setButtonDrawable(drawable.button);
        joystick.setListener(this);

        aButton = m_inputView.findViewById(a_button);
        aButton.setOnTouchListener(aButtonListener);
        aButton.setAlpha(0.5f);

        bButton = m_inputView.findViewById(b_button);
        bButton.setOnTouchListener(bButtonListener);
        bButton.setAlpha(0.5f);

        cButton = m_inputView.findViewById(c_button);
        cButton.setOnTouchListener(cButtonListener);
        cButton.setAlpha(0.5f);

    }

    View.OnTouchListener aButtonListener = new View.OnTouchListener() {
        public boolean onTouch( View yourButton , MotionEvent theMotion ) {
            switch ( theMotion.getAction() ) {
                case MotionEvent.ACTION_DOWN:
                {
                    aButton.setPressed(true);
                    SDLActivity.onNativeKeyDown(KeyEvent.KEYCODE_SPACE);
                }
                break;
                case MotionEvent.ACTION_UP:
                {
                    aButton.setPressed(false);
                    SDLActivity.onNativeKeyUp(KeyEvent.KEYCODE_SPACE);
                }
                break;

            }
            return true;
        }
    };

    View.OnTouchListener bButtonListener = new View.OnTouchListener() {
        public boolean onTouch( View yourButton , MotionEvent theMotion ) {
            switch ( theMotion.getAction() ) {
                case MotionEvent.ACTION_DOWN:
                {
                    bButton.setPressed(true);
                    SDLActivity.onNativeKeyDown(KeyEvent.KEYCODE_ENTER);
                }
                break;
                case MotionEvent.ACTION_UP:
                {
                    bButton.setPressed(false);
                    SDLActivity.onNativeKeyUp(KeyEvent.KEYCODE_ENTER);
                }
                break;

            }
            return true;
        }
    };


    View.OnTouchListener cButtonListener = new View.OnTouchListener() {
        boolean isJump = false;
        boolean isSitDown = false;
        public boolean onTouch(View yourButton, MotionEvent theMotion) {

            switch (theMotion.getAction()) {
                case MotionEvent.ACTION_DOWN: {

                    cButton.setPressed(true);
                    SDLActivity.onNativeKeyDown(KeyEvent.KEYCODE_ESCAPE);
                }
                break;
                case MotionEvent.ACTION_UP:
                {
                    cButton.setPressed(false);
                    SDLActivity.onNativeKeyUp(KeyEvent.KEYCODE_ESCAPE);
                }
                break;

            }
            return true;
        }
    };

    @Override
    public void onMove(JoyStick joyStick, double angle, double power, int direction) {
        switch (joyStick.getId()) {
            case joy1:
            {
                int joystickThrehold = 40;
                int keyEvent = -1;
                int keyCode = -1;
                int dir = joyStick.getDirection();
                if((dir == DIRECTION_CENTER ) || power < joystickThrehold)
                    keyEvent = ACTION_UP;
                else if(power >= joystickThrehold)
                    keyEvent = KeyEvent.ACTION_DOWN;

                if(KeyEvent.ACTION_DOWN == keyEvent)
                {
                    if(dir == DIRECTION_LEFT)
                        keyCode = KeyEvent.KEYCODE_DPAD_LEFT;
                    if(dir == DIRECTION_UP)
                        keyCode = KeyEvent.KEYCODE_DPAD_UP;
                    if(dir == DIRECTION_RIGHT)
                        keyCode = KeyEvent.KEYCODE_DPAD_RIGHT;
                    if(dir == DIRECTION_DOWN)
                        keyCode = KeyEvent.KEYCODE_DPAD_DOWN;

                    boolean ignoreKey = false;
                    if (keyEvent == KeyEvent.ACTION_DOWN && keyCode != -1) {

                        if(keyStates[KEY_LEFT] == 1 || keyStates[KEY_RIGHT] == 1)
                        {
                            if(keyCode == KeyEvent.KEYCODE_DPAD_DOWN ||  keyCode == KeyEvent.KEYCODE_DPAD_UP)
                                ignoreKey = true;
                        }

                        if(ignoreKey == false)
                        {
                            if(keyStates[KEY_LEFT] == 1 && keyCode != KeyEvent.KEYCODE_DPAD_LEFT) {
                                keyStates[KEY_LEFT] = 0;
                                SDLActivity.onNativeKeyUp(KeyEvent.KEYCODE_DPAD_LEFT);
                            }
                            if(keyStates[KEY_RIGHT] == 1&& keyCode != KeyEvent.KEYCODE_DPAD_RIGHT) {
                                keyStates[KEY_RIGHT] = 0;
                                SDLActivity.onNativeKeyUp(KeyEvent.KEYCODE_DPAD_RIGHT);
                            }
                            if(keyStates[KEY_UP] == 1 && keyCode != KeyEvent.KEYCODE_DPAD_UP) {
                                keyStates[KEY_UP] = 0;
                                SDLActivity.onNativeKeyUp(KeyEvent.KEYCODE_DPAD_UP);
                            }
                            if(keyStates[KEY_DOWN] == 1 && keyCode != KeyEvent.KEYCODE_DPAD_DOWN) {
                                keyStates[KEY_DOWN] = 0;
                                SDLActivity.onNativeKeyUp(KeyEvent.KEYCODE_DPAD_DOWN);
                            }

                            if(keyStates[KEY_LEFT] == 0 && keyCode == KeyEvent.KEYCODE_DPAD_LEFT) {
                                keyStates[KEY_LEFT] = 1;
                                SDLActivity.onNativeKeyDown(KeyEvent.KEYCODE_DPAD_LEFT);
                                userDirection = DIRECTION_LEFT;
                            }
                            if(keyStates[KEY_RIGHT] == 0 && keyCode == KeyEvent.KEYCODE_DPAD_RIGHT) {
                                keyStates[KEY_RIGHT] = 1;
                                SDLActivity.onNativeKeyDown(KeyEvent.KEYCODE_DPAD_RIGHT);
                                userDirection = DIRECTION_RIGHT;
                            }
                            if(keyStates[KEY_UP] == 0 && keyCode == KeyEvent.KEYCODE_DPAD_UP) {
                                keyStates[KEY_UP] = 1;
                                SDLActivity.onNativeKeyDown(KeyEvent.KEYCODE_DPAD_UP);
                            }
                            if(keyStates[KEY_DOWN] == 0 && keyCode == KeyEvent.KEYCODE_DPAD_DOWN) {
                                keyStates[KEY_DOWN] = 1;
                                SDLActivity.onNativeKeyDown(KeyEvent.KEYCODE_DPAD_DOWN);
                            }

                        }
                    }


                }
                else if (keyEvent == ACTION_UP) {
                    if(keyStates[KEY_LEFT] == 1) {
                        keyStates[KEY_LEFT] = 0;
                        SDLActivity.onNativeKeyUp(KeyEvent.KEYCODE_DPAD_LEFT);
                    }
                    if(keyStates[KEY_RIGHT] == 1) {
                        keyStates[KEY_RIGHT] = 0;
                        SDLActivity.onNativeKeyUp(KeyEvent.KEYCODE_DPAD_RIGHT);
                    }
                    if(keyStates[KEY_UP] == 1) {
                        keyStates[KEY_UP] = 0;
                        SDLActivity.onNativeKeyUp(KeyEvent.KEYCODE_DPAD_UP);
                    }
                    if(keyStates[KEY_DOWN] == 1) {
                        keyStates[KEY_DOWN] = 0;
                        SDLActivity.onNativeKeyUp(KeyEvent.KEYCODE_DPAD_DOWN);
                    }
                }
            }
            break;
        }
    }

    @Override
    public void onTap()
    {

    }

    @Override
    public void onDoubleTap()
    {
        SDLActivity.onNativeKeyDown(KeyEvent.KEYCODE_ESCAPE);
    }

    /*@Override
    public void onBackPressed() {

        android.support.v7.app.AlertDialog.Builder builder = new android.support.v7.app.AlertDialog.Builder(this);

        builder.setPositiveButton("sdf", new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialogInterface, int i) {
                //finish();
            }
        });

        builder.setNegativeButton("sdf", new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialogInterface, int i) {
                return;
            }
        });

        android.support.v7.app.AlertDialog alterDialog = builder.create();
        alterDialog.setTitle("sdf");
        alterDialog.setMessage("sdf");
        alterDialog.show();

    }*/

}
