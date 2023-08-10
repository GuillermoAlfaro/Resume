from models.model import Model
from keras.models import Sequential
from keras.layers import MaxPooling2D, Flatten, Dense, Conv2D, RandomRotation, Activation, RandomFlip, BatchNormalization
from keras.optimizers import RMSprop
from keras import regularizers

class BasicModel(Model):
    def _define_model(self, input_shape, categories_count):
        model = Sequential(
            [
                Conv2D(16, (3, 3), activation='relu', input_shape=input_shape),
                BatchNormalization(),
                MaxPooling2D(pool_size=(2, 2)),

                Conv2D(32, (3, 3), activation='relu'),
                BatchNormalization(),
                MaxPooling2D(pool_size=(2, 2)),

                Conv2D(64, (3, 3), activation='relu'),
                BatchNormalization(),
                MaxPooling2D(pool_size=(2, 2)),

                Conv2D(128, (3, 3), activation='relu'),
                BatchNormalization(),
                MaxPooling2D(pool_size=(2, 2)),
                Flatten(),

                Dense(512, activation='relu'),
                BatchNormalization(),

                Dense(128, activation='relu'),
                BatchNormalization(),

                Dense(categories_count, activation='softmax'),
            ]
        )

        self.model = model

    def _compile_model(self):
        self.model.compile(
            optimizer=RMSprop(learning_rate=0.001),
            loss='categorical_crossentropy',
            metrics=['accuracy'],
        )
