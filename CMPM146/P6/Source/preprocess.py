from keras.utils import image_dataset_from_directory
from config import train_directory, test_directory, image_size, batch_size, validation_split
import tensorflow as tf

def _split_data(train_directory, test_directory, batch_size, validation_split):
    print('train dataset:')
    train_dataset, validation_dataset = image_dataset_from_directory(
        train_directory,
        label_mode='categorical',
        color_mode='rgb',
        batch_size=batch_size,
        image_size=image_size,
        validation_split=validation_split,
        subset="both",
        seed=47
    )
    print('test dataset:')
    test_dataset = image_dataset_from_directory(
        test_directory,
        label_mode='categorical',
        color_mode='rgb',
        batch_size=batch_size,
        image_size=image_size,
        shuffle=False
    )

    return train_dataset, validation_dataset, test_dataset

def _perform_augmentation(dataset, function):
    return dataset.map(lambda x, y: (function(x), y))

def _augment_dataset(dataset):
    dataset = _perform_augmentation(dataset, tf.image.random_flip_left_right)
    #dataset = _perform_augmentation(dataset, tf.keras.layers.RandomFlip)
    return dataset

def get_datasets():
    train_dataset, validation_dataset, test_dataset = \
        _split_data(train_directory, test_directory, batch_size, validation_split)

    train_dataset = _augment_dataset(train_dataset)

    return train_dataset, validation_dataset, test_dataset