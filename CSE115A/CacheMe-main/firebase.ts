
import * as firebase from "firebase/app";
import { getFirestore } from "firebase/firestore";
import { getStorage } from "firebase/storage";
import { getAuth, createUserWithEmailAndPassword, signInWithEmailAndPassword } from "firebase/auth";
import {REACT_APP_API_KEY, REACT_APP_AUTH_DOMAIN,
  REACT_APP_PROJECT_ID, REACT_APP_STORAGE_BUCKET,
  REACT_APP_MESSAGING_SENDER_ID, 
  REACT_APP_APP_ID, REACT_APP_MEASUREMENT_ID} from '@env'

const firebaseConfig = {
  apiKey: REACT_APP_API_KEY,
  authDomain: REACT_APP_AUTH_DOMAIN,
  projectId: REACT_APP_PROJECT_ID,
  storageBucket: REACT_APP_STORAGE_BUCKET,
  messagingSenderId: REACT_APP_MESSAGING_SENDER_ID,
  appId: REACT_APP_APP_ID,
  measurementId: REACT_APP_MEASUREMENT_ID
};


let app;
if(firebase.getApps().length === 0){
    app = firebase.initializeApp(firebaseConfig);
}else {
    app = firebase.getApp();
}

const auth = getAuth(app);
const db = getFirestore(app);
const storage = getStorage(app);

export { auth, db, storage, createUserWithEmailAndPassword, signInWithEmailAndPassword };
