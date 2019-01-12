package com.example.vadym.medsuccess

import android.content.Intent
import android.support.v7.app.AppCompatActivity
import android.os.Bundle
import android.util.Log
import android.widget.Button
import android.widget.EditText
import android.widget.TextView

class LoginActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_login)

        val loginButton = findViewById<Button>(R.id.login_button)
        loginButton.setOnClickListener {
            val username = findViewById<EditText>(R.id.username).text.toString()
            val password = findViewById<EditText>(R.id.password).text.toString()

            Log.e("login", username)

            if (username == "doctor" && password == "12345678") {
                token = "82f66a158ef0997dd0a8e3abc7bc1374f9dbb018"
                Log.e("login", "OK")
                val i = Intent(this, RecipeListActivity::class.java)
                startActivity(i)
            }
            else {
                findViewById<TextView>(R.id.login_err_msg).text = "Wrong credentials! Try again."
            }
        }
    }
}
