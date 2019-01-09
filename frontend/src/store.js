import Vue from "vue";
import Vuex from "vuex";
import axios from 'axios';

Vue.use(Vuex);

export default new Vuex.Store({
  state: {
    status: '',
    token: localStorage.getItem('token') || '',
    id: 0
  },
  mutations: {
    auth_request(state) {
      state.status = 'loading';
    },
    auth_success(state, token, id) {
      state.status = 'success';
      state.token = token;
      state.id = id;
    },
    auth_error(state) {
      state.status = 'error';
    },
    logout(state) {
      state.status = '';
      state.token = '';
    },
  },
  actions: {
    login({commit}, user) {
      return new Promise((resolve, reject) => {
        commit('auth_request');
        axios({url: 'http://localhost:8000/users/login/', data: user, method: 'POST'})
          .then(resp => {
            const token = resp.data.token;
            const id = resp.data.id;
            localStorage.setItem("token", token);
            localStorage.setItem("id", id);
            axios.defaults.headers.common['Authorization'] = "Token " + token;
            commit('auth_success', token, id);
            resolve(resp);
          })
          .catch(err => {
            commit('auth_error');
            localStorage.removeItem("token");
            localStorage.removeItem("id");
            reject(err);
          })
      });
    },
    register({commit}, user){
      return new Promise((resolve, reject) => {
        commit('auth_request')
        axios({url: 'http://localhost:8000/users/', data: user, method: 'POST' })
          .then(resp => {
            const token = resp.data.token;
            const id = resp.data.id;
            localStorage.setItem("token", token);
            localStorage.setItem("id", id);
            axios.defaults.headers.common['Authorization'] = "Token " + token;
            commit('auth_success', token, id);
            resolve(resp);
          })
          .catch(err => {
            commit('auth_error', err);
            localStorage.removeItem('token');
            reject(err);
          })
      })
    },
    logout({commit}){
      return new Promise((resolve, reject) => {
        commit('logout');
        localStorage.removeItem("token");
        localStorage.removeItem("id");
        delete axios.defaults.headers.common["Authorization"];
        resolve();
      });
    }
  },
  getters: {
    isLoggedIn: state => !!state.token
  }
});
