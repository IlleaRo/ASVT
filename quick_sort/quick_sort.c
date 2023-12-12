void quickSort(unsigned char *Arr, int left, int right) {
    int pivot; // разрешающий элемент
    int l_hold = left; //левая граница
    int r_hold = right; // правая граница
    pivot = Arr[left];
    while (left < right) // пока границы не сомкнутся
    {
        while ((Arr[right] >= pivot) && (left < right))
            right--; // сдвигаем правую границу пока элемент [right] больше [pivot]
        if (left != right) // если границы не сомкнулись
        {
            Arr[left] = Arr[right]; // перемещаем элемент [right] на место разрешающего
            left++; // сдвигаем левую границу вправо
        }
        while ((Arr[left] <= pivot) && (left < right))
            left++; // сдвигаем левую границу пока элемент [left] меньше [pivot]
        if (left != right) // если границы не сомкнулись
        {
            Arr[right] = Arr[left]; // перемещаем элемент [left] на место [right]
            right--; // сдвигаем правую границу влево
        }
    }
    Arr[left] = pivot; // ставим разрешающий элемент на место
    pivot = left;
    left = l_hold;
    right = r_hold;
    if (left < pivot) // Рекурсивно вызываем сортировку для левой и правой части массива
        quickSort(Arr, left, pivot - 1);
    if (right > pivot)
        quickSort(Arr, pivot + 1, right);
}

void sort(unsigned char *Arr, int size)
{
    quickSort(Arr, 0, size - 1);
}