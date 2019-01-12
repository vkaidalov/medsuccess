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

class RecipeDetailActivity:Activity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_recipe_detail)

        val recipe_id = intent.getStringExtra("recipe_id")

        val o =
            createRequest("$domain/recipes/$recipe_id/")
                .map { Gson().fromJson(it, RecipeDetailAPI::class.java) }
                .subscribeOn(Schedulers.io()).observeOn(AndroidSchedulers.mainThread())

        val request = o.subscribe({
            showRecipeDetail(it)
        }, {
            Log.e("tag", "", it)
        })
    }

    fun showRecipeDetail(recipeDetail: RecipeDetailAPI) {
        findViewById<TextView>(R.id.recipe_id).text = "Recipe ID: " + recipeDetail.id
        findViewById<TextView>(R.id.medicine).text = "Medicine: " + recipeDetail.medicine
        findViewById<TextView>(R.id.reason).text = "Reason: " + recipeDetail.reason
        findViewById<TextView>(R.id.doctor).text = "Doctor ID: " + recipeDetail.doctor.toString()

        val inflater = layoutInflater
        for (dose in recipeDetail.doses) {
            val list = findViewById<LinearLayout>(R.id.dose_list)
            val view = inflater.inflate(R.layout.dose_item, list, false)
            view.findViewById<TextView>(R.id.dose_id).text = "Dose ID: " + dose.id
            view.findViewById<TextView>(R.id.date_assigned).text = "Assigned: " + dose.date_assigned
            view.findViewById<TextView>(R.id.date_consumed).text = "Consumed: " + dose.date_consumed
            view.findViewById<TextView>(R.id.quantity).text = "Quantity: " + dose.quantity
            list.addView(view)
        }
    }
}

class DoseDetailAPI(
    val id: Int,
    val date_assigned: String,
    val date_consumed: String,
    val quantity: String
)

class RecipeDetailAPI(
    val id: Int,
    val medicine: String,
    val reason: String,
    val doctor: Int,
    val doses: ArrayList<DoseDetailAPI>
)