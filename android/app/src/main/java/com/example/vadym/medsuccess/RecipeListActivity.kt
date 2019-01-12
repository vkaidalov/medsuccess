package com.example.vadym.medsuccess

import android.app.Activity
import android.content.Intent
import android.os.Bundle
import android.util.Log
import android.widget.LinearLayout
import android.widget.TextView
import com.google.gson.Gson
import io.reactivex.android.schedulers.AndroidSchedulers
import io.reactivex.schedulers.Schedulers

class RecipeListActivity:Activity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_recipe_list)

        val o =
            createRequest("$domain/recipes/")
                .map { Gson().fromJson(it, RecipeListAPI::class.java) }
                .subscribeOn(Schedulers.io()).observeOn(AndroidSchedulers.mainThread())

        val request = o.subscribe({
            showLinearLayout(it)
        }, {
            Log.e("tag", "", it)
        })
    }

    fun showLinearLayout(recipeList:RecipeListAPI) {
        val inflater = layoutInflater
        for (recipe in recipeList) {
            val list = findViewById<LinearLayout>(R.id.recipe_list)
            val view = inflater.inflate(R.layout.recipe_item, list, false)
            view.findViewById<TextView>(R.id.recipe_id).text = "Recipe ID: " + recipe.id
            view.findViewById<TextView>(R.id.medicine).text = "Medicine: " + recipe.medicine
            view.findViewById<TextView>(R.id.reason).text = "Reason: " + recipe.reason
            view.findViewById<TextView>(R.id.doctor).text = "Doctor ID: " + recipe.doctor.toString()
            view.setOnClickListener {
                Log.e("CLICK", recipe.id.toString())
                val i = Intent(this, RecipeDetailActivity::class.java)
                i.putExtra("recipe_id", recipe.id.toString())
                startActivity(i)
            }
            list.addView(view)
        }
    }
}

class RecipeListAPI:ArrayList<RecipeItemAPI>(){
}

class RecipeItemAPI(
    val id: Int,
    val date_created: String,
    val medicine: String,
    val reason: String,
    val doctor: Int
)